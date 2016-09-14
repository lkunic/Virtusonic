// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "FretFinger/FretFinger.h"

#include "FretFingerRestAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UFretFingerRestAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AFretFinger *fretFinger);

	float GetAnimationLength();

	virtual void Execute() override;

private:
	UPROPERTY()
	AFretFinger *mFretFinger;

	UPROPERTY()
	UAnimSequence* mAnimation;
};
