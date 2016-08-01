// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Pick.h"


// Sets default values
APick::APick()
{
	PrimaryActorTick.bCanEverTick = false;

	// Add an pick animator component to the actor
	_animator = CreateDefaultSubobject<UPickAnimator>(TEXT("PickAnimator"));
}

/*
 * Pick initialization. Stores the song metadata, generates the internal timeline used when generating the pick actions.
 */
void APick::Init(int32 tempo, int32 ticksPerQuarter, int32 lastTick, FString stringRoots)
{
	_tempo = tempo;
	_ticksPerQuarter = ticksPerQuarter;
	_timePerTick = 60.0f / (tempo * ticksPerQuarter);;
	_timelineLength = lastTick + ticksPerQuarter * 4;

	// Allocate the memory for the internal timeline and initialize it with 'R' (rest)
	_timelineStatus = (TCHAR*)malloc(sizeof(TCHAR) * (_timelineLength));
	for (int i = 0; i < _timelineLength; i++)
	{
		_timelineStatus[i] = 'R';
	}

	_stringRoots = stringRoots;
}

/// GENERATING PICK ACTIONS ///

/*
 * Check if the given note can be played.
 * The pick can't be playing and it needs to be able to perform the preparation animation 
 * unless it is already at the correct string.
 */
bool APick::CanPlayNote(int32 tick, int32 stringIndex)
{
	return !IsPlaying(tick) &&
		((IsResting(tick) && CanAnimateFromRest(tick, stringIndex)) ||
		IsOnCorrectString(tick, stringIndex) ||
		CanAnimateFromString(tick, stringIndex));
}

/*
 * Check if the pick is in an animation at the given tick.
 */
bool APick::IsPlaying(int32 tick)
{
	return _timelineStatus[tick] == 'P';
}

/*
 * Check if the pick is in the rest position at the given tick.
 */
bool APick::IsResting(int32 tick)
{
	return _timelineStatus[tick] == 'R';
}

/*
 * Check if the pick is prepared to play the correct string at the given tick.
 */
bool APick::IsOnCorrectString(int32 tick, int32 stringIndex)
{
	return  _timelineStatus[tick] == _stringRoots[stringIndex];
}

/*
 * Check if the pick has enough time to animate from the rest position until the given tick.
 */
bool APick::CanAnimateFromRest(int32 tick, int32 stringIndex)
{
	UAnimSequence* anim = _animator->GetAnimationSequence(EPickAnimations::RestToPickReadyX, _stringRoots[stringIndex]);

	for (int i = tick - SequenceLengthInTicks(anim->SequenceLength / anim->RateScale); i < tick; i++)
	{
		if (_timelineStatus[i] != 'R')
		{
			return false;
		}
	}

	return true;
}

/*
 * Check if the pick has enough time to animate from the current string position until the given tick.
 */
bool APick::CanAnimateFromString(int32 tick, int32 stringIndex)
{
	UAnimSequence* anim = _animator->GetAnimationSequence(EPickAnimations::PickReadyXToPickReadyY, _timelineStatus[tick], _stringRoots[stringIndex]);

	for (int i = tick - SequenceLengthInTicks(anim->SequenceLength / anim->RateScale); i < tick; i++)
	{
		if (_timelineStatus[i] != _timelineStatus[tick])
		{
			return false;
		}
	}
	
	return true;
}

/*
 * Check if the pick has enough time to go back to the rest position and then prepare for the new note until the given tick.
 */
bool APick::CanRestBeforePick(int32 tick)
{
	if (_timelineStatus[tick] == 'R')
	{
		// Already resting
		return false;
	}

	UAnimSequence* stringToRestAnim = _animator->GetAnimationSequence(EPickAnimations::PickReadyXToRest, _timelineStatus[tick]);
	UAnimSequence* restToStringAnim = _animator->GetAnimationSequence(EPickAnimations::RestToPickReadyX, _timelineStatus[tick]);

	for (int i = tick - SequenceLengthInTicks(stringToRestAnim->SequenceLength / stringToRestAnim->RateScale) -
		SequenceLengthInTicks(restToStringAnim->SequenceLength / restToStringAnim->RateScale); i < tick; i++)
	{
		if (_timelineStatus[i] != _timelineStatus[tick])
		{
			return false;
		}
	}

	return true;
}

/*
 * Updates the internal timeline with the given status flag.
 */
void APick::UpdateTimeline(int32 startTick, int32 endTick, TCHAR status)
{
	for (int i = startTick; i < endTick; i++)
	{
		_timelineStatus[i] = status;
	}
}

/*
 * Returns the reference to the pick animator component.
 */
UPickAnimator* APick::GetAnimator()
{
	return _animator;
}

/*
 * Returns the tick after the last time the pick was in an animation, or 0 if the pick hadn't moved from the resting position.
 */
int32 APick::GetLastPlayTick()
{
	for (int i = _timelineLength - 1; i >= 0; i--)
	{
		if (_timelineStatus[i] == 'P')
		{
			return i + 1;
		}
	}

	return 0;
}

/*
 * Converts the given float sequence duration (in seconds) into ticks.
 */
int32 APick::SequenceLengthInTicks(float animSequenceLength)
{
	return FMath::CeilToInt(animSequenceLength / _timePerTick);
}

/*
 * Returns the timeline status at the given tick.
 */
TCHAR APick::TimelineStatus(int32 tick)
{
	return _timelineStatus[tick];
}