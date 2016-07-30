// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Pick/Pick.h"

#include "PickPrepareAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UPickPrepareAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	void Init(APick* pick, TCHAR fromString, TCHAR toString);

	float GetAnimationLength();
	void SetAnimationRate(float animationRate);

	virtual void Execute() override;

private:
	UPROPERTY()
	APick* _pick;

	UPROPERTY()
	UAnimSequence* _animation;

	float _animationRate;
};
