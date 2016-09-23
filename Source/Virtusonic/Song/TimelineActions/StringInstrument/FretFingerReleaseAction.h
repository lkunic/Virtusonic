// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "FretFinger/FretFinger.h"

#include "FretFingerReleaseAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UFretFingerReleaseAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AFretFinger *fretFinger, int8 string, int32 noteStartTick, float releaseDuration);

	virtual void Execute() override;

private:
	UPROPERTY()
	AFretFinger *mFretFinger;

	int8 mString;
	int32 mNoteStartTick;
	float mReleaseDuration;
};
