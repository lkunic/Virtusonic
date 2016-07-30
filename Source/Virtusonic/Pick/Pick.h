// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "PickAnimator.h"

#include "Pick.generated.h"

UENUM()
enum class EPickStatus : uint8
{
	Rest,
	Preparing,
	Ready,
	Picking,
	End
};

UCLASS()
class VIRTUSONIC_API APick : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APick();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPickAnimator* GetAnimator();

	void Init(int32 tempo, int32 ticksPerQuarter, int32 lastTick, FString stringRoots);

	bool CanPlayNote(int32 tick, int32 stringIndex);

	bool IsPlaying(int32 tick);
	bool IsResting(int32 tick);
	bool IsOnCorrectString(int32 tick, int32 stringIndex);
	bool CanAnimateFromRest(int32 tick, int32 stringIndex);
	bool CanAnimateFromString(int32 tick, int32 stringIndex);
	bool CanRestBeforePick(int32 tick);

	void UpdateTimeline(int32 startTick, int32 endTick, TCHAR status);
	int32 GetLastPlayTick();
	int32 SequenceLengthInTicks(float animSequenceLength);
	TCHAR TimelineStatus(int32 tick);

private:

	UPickAnimator* _animator;
	TCHAR* _timelineStatus;
	EPickStatus _pickStatus;
	FString _stringRoots;

	int32 _tempo;
	int32 _ticksPerQuarter;
	int32 _timelineLength;
	float _timePerTick;
};
