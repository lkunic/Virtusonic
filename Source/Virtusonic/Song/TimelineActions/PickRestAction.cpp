// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickRestAction.h"

/*
 * Initializes the action with the given pick reference and the current string position.
 */
void UPickRestAction::Init(APick *pick, TCHAR fromString)
{
	mPick = pick;
	mAnimation = mPick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToRest, fromString);
}

/*
 * Returns the animation length.
 */
float UPickRestAction::GetAnimationLength()
{
	return mAnimation->SequenceLength / mAnimation->RateScale;
}

/*
 * Starts the pick rest animation.
 */
void UPickRestAction::Execute()
{
	mPick->GetSkeletalMeshComponent()->Stop();
	mPick->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
}


