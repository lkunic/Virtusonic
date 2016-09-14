// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerMoveAction.h"

void UFretFingerMoveAction::Init(AFretFinger *fretFinger, int8 targetFret, float prepareDuration)
{
	mFretFinger = fretFinger;
	mTargetFret = targetFret;
	mPrepareDuration = prepareDuration;
}

void UFretFingerMoveAction::Execute()
{
	mFretFinger->GetAnimator()->MoveToFret(mTargetFret, mPrepareDuration);
}


