// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Pick/Pick.h"

#include "PickRestAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UPickRestAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(APick* pick, TCHAR fromString);

	float GetAnimationLength();

	virtual void Execute() override;

private:
	UPROPERTY()
	APick* _pick;

	UPROPERTY()
	UAnimSequence* _animation;
};
