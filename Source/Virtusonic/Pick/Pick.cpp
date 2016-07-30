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

// Called when the game starts or when spawned
void APick::BeginPlay()
{
	Super::BeginPlay();
}

UPickAnimator* APick::GetAnimator()
{
	return _animator;
}

void APick::Init(int32 tempo, int32 ticksPerQuarter, int32 lastTick, FString stringRoots)
{
	_tempo = tempo;
	_ticksPerQuarter = ticksPerQuarter;
	_timePerTick = 60.0f / (tempo * ticksPerQuarter);;
	_timelineLength = lastTick + ticksPerQuarter * 4;

	_timelineStatus = (TCHAR*)malloc(sizeof(TCHAR) * (_timelineLength));

	for (int i = 0; i < _timelineLength; i++)
	{
		_timelineStatus[i] = 'R';
	}

	_pickStatus = EPickStatus::Rest;
	_stringRoots = stringRoots;
}

bool APick::CanPlayNote(int32 tick, int32 stringIndex)
{
	return !IsPlaying(tick) &&
		((IsResting(tick) && CanAnimateFromRest(tick, stringIndex)) ||
		IsOnCorrectString(tick, stringIndex) ||
		CanAnimateFromString(tick, stringIndex));
}

bool APick::IsPlaying(int32 tick)
{
	return _timelineStatus[tick] == 'P';
}

bool APick::IsResting(int32 tick)
{
	return _timelineStatus[tick] == 'R';
}

bool APick::IsOnCorrectString(int32 tick, int32 stringIndex)
{
	return  _timelineStatus[tick] == _stringRoots[stringIndex];
}

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

void APick::UpdateTimeline(int32 startTick, int32 endTick, TCHAR status)
{
	for (int i = startTick; i < endTick; i++)
	{
		_timelineStatus[i] = status;
	}
}

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

int32 APick::SequenceLengthInTicks(float animSequenceLength)
{
	return FMath::CeilToInt(animSequenceLength / _timePerTick);
}

TCHAR APick::TimelineStatus(int32 tick)
{
	return _timelineStatus[tick];
}