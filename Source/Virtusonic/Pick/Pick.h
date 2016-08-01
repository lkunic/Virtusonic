// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "PickAnimator.h"

#include "Pick.generated.h"

/**
 * The actor for controlling picks.
 */
UCLASS()
class VIRTUSONIC_API APick : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	APick();

	void Init(int32 tempo, int32 ticksPerQuarter, int32 lastTick, FString stringRoots);

	// Functions used for generating the pick actions
	bool CanPlayNote(int32 tick, int32 stringIndex);
	bool IsPlaying(int32 tick);
	bool IsResting(int32 tick);
	bool IsOnCorrectString(int32 tick, int32 stringIndex);
	bool CanAnimateFromRest(int32 tick, int32 stringIndex);
	bool CanAnimateFromString(int32 tick, int32 stringIndex);
	bool CanRestBeforePick(int32 tick);
	void UpdateTimeline(int32 startTick, int32 endTick, TCHAR status);

	UPickAnimator* GetAnimator();
	int32 GetLastPlayTick();
	int32 SequenceLengthInTicks(float animSequenceLength);
	TCHAR TimelineStatus(int32 tick);

private:

	UPickAnimator* _animator;
	TCHAR* _timelineStatus;
	FString _stringRoots;

	int32 _tempo;
	int32 _ticksPerQuarter;
	int32 _timelineLength;
	float _timePerTick;
};
