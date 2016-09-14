// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerPrepareAction.h"

void UFretFingerPrepareAction::Init(AFretFinger *fretFinger)
{
	mFretFinger = fretFinger;
	mAnimation = mFretFinger->GetAnimator()->GetAnimationSequence(EFretFingerAnimation::FretFingerPrepare);
}

/*
* Returns the length of the fret finger prepare animation.
*/
float UFretFingerPrepareAction::GetAnimationLength()
{
	return mAnimation->SequenceLength / mAnimation->RateScale;
}

void UFretFingerPrepareAction::Execute()
{
	mFretFinger->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
}

