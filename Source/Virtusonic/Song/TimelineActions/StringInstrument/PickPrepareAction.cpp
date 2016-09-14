// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickPrepareAction.h"

/*
 * Initializes the action with the given pick reference, the current string position and the final position.
 */
void UPickPrepareAction::Init(APick *pick, TCHAR fromString, TCHAR toString)
{
	mPick = pick;

	if (fromString == 'R')
	{
		// The string is currently in resting position
		mAnimation = mPick->GetAnimator()->GetAnimationSequence(EPickAnimations::RestToPickReadyX, toString);
	}
	else
	{
		mAnimation = mPick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToPickReadyY, fromString, toString);
	}

	// By default, the animation rate is 1.0
	mAnimationRate = 1.0f;
}

/*
 * Returns the length of the pick preparation animation.
 */
float UPickPrepareAction::GetAnimationLength()
{
	return mAnimation->SequenceLength / mAnimation->RateScale;
}

/*
 * Sets the rate of the preparation animation. The default value is 1.
 */
void UPickPrepareAction::SetAnimationRate(float animationRate)
{
	mAnimationRate = animationRate;
}

/*
 * Starts the pick preparation animation.
 */
void UPickPrepareAction::Execute()
{
	mAnimation->RateScale = mAnimationRate;
	mPick->GetSkeletalMeshComponent()->Stop();
	mPick->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
}

