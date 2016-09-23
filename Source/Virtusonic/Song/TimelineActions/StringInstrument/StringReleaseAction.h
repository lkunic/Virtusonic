// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "String/String.h"

#include "StringReleaseAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UStringReleaseAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AString *string, int32 noteStartTick, float releaseDuration);

	virtual void Execute() override;

private:
	UPROPERTY()
	AString *mString;

	int32 mNoteStartTick;
	float mReleaseDuration;
};
