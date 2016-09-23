// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerRestAction.h"

void UFretFingerRestAction::Init(AFretFinger *fretFinger)
{
	mFretFinger = fretFinger;
	mAnimation = mFretFinger->GetAnimator()->GetAnimationSequence(EFretFingerAnimations::FretFingerRest);
}

/*
* Returns the length of the fret finger rest animation.
*/
float UFretFingerRestAction::GetAnimationLength()
{
	return mAnimation->SequenceLength / mAnimation->RateScale;
}

void UFretFingerRestAction::Execute()
{
	mFretFinger->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
	mFretFinger->GetAnimator()->ReturnToRest();
}




