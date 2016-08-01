// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Pick/Pick.h"

#include "PickPrepareAction.generated.h"

/**
* The song timeline action for moving the pick from its current position to the string it needs to play.
*/
UCLASS()
class VIRTUSONIC_API UPickPrepareAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	// Initializes the action with the given pick reference, the current string position and the final position
	void Init(APick* pick, TCHAR fromString, TCHAR toString);

	float GetAnimationLength();

	// Allows changing the animation rate so the animations can be sped up as necessary
	void SetAnimationRate(float animationRate);

	virtual void Execute() override;

private:
	UPROPERTY()
	APick* _pick;

	UPROPERTY()
	UAnimSequence* _animation;

	float _animationRate;
};
