// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickPlayAction.h"

/*
 * Initializes the action with the given pick reference and the current string position.
 */
void UPickPlayAction::Init(APick *pick, TCHAR stringRoot)
{
	mPick = pick;
	mAnimation = mPick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickX, stringRoot);
}

/*
 * Returns the length of the pick play animation.
 */
float UPickPlayAction::GetAnimationLength()
{
	return mAnimation->SequenceLength / mAnimation->RateScale;
}

/*
 * Starts the pick play animation.
 */
void UPickPlayAction::Execute()
{
	mPick->GetSkeletalMeshComponent()->Stop();
	mPick->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
	//UE_LOG(VirtusonicLog, Log, TEXT("Playing pick %s"), *(_pick->GetName()));
}



