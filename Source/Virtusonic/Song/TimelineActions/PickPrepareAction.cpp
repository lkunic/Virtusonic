// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickPrepareAction.h"

/*
 * Initializes the action with the given pick reference, the current string position and the final position.
 */
void UPickPrepareAction::Init(APick* pick, TCHAR fromString, TCHAR toString)
{
	_pick = pick;

	if (fromString == 'R')
	{
		// The string is currently in resting position
		_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::RestToPickReadyX, toString);
	}
	else
	{
		_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToPickReadyY, fromString, toString);
	}

	// By default, the animation rate is 1.0
	_animationRate = 1.0f;
}

/*
 * Returns the length of the pick preparation animation.
 */
float UPickPrepareAction::GetAnimationLength()
{
	return _animation->SequenceLength / _animation->RateScale;
}

/*
 * Sets the rate of the preparation animation. The default value is 1.
 */
void UPickPrepareAction::SetAnimationRate(float animationRate)
{
	_animationRate = animationRate;
}

/*
 * Starts the pick preparation animation.
 */
void UPickPrepareAction::Execute()
{
	_animation->RateScale = _animationRate;
	_pick->GetSkeletalMeshComponent()->Stop();
	_pick->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
}

