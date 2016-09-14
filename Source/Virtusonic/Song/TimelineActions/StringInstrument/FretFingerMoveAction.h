// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "FretFinger/FretFinger.h"

#include "FretFingerMoveAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UFretFingerMoveAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	void Init(AFretFinger *fretFinger, int8 targetFret, float prepareDuration);

	virtual void Execute() override;

private:
	UPROPERTY()
	AFretFinger *mFretFinger;

	int8 mTargetFret;

	float mPrepareDuration;
};
