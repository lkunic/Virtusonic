// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "FretFinger/FretFinger.h"

#include "FretFingerPressAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UFretFingerPressAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AFretFinger *fretFinger, int8 string, int32 noteStartTick, float pressDuration);

	virtual void Execute() override;

private:
	UPROPERTY()
	AFretFinger *mFretFinger;

	int8 mString;
	int32 mNoteStartTick;
	float mPressDuration;
};
