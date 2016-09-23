// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "String/String.h"

#include "StringPressAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UStringPressAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AString *string, int8 fret, int32 noteStartTick, float pressDuration);

	virtual void Execute() override;

private:
	UPROPERTY()
	AString *mString;
	
	int8 mFret;
	int32 mNoteStartTick;
	float mPressDuration;	
};
