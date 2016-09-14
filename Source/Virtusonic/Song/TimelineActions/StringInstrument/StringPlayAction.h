// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "String/String.h"

#include "StringPlayAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UStringPlayAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	// Initializes the action with the given string reference and the current string position
	void Init(AString *string, TCHAR stringRoot);

	virtual void Execute() override;

private:
	UPROPERTY()
	AString *mString;

	UPROPERTY()
	UAnimSequence *mAnimation;
};
