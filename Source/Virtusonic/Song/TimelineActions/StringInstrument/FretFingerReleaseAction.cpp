// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerReleaseAction.h"

void UFretFingerReleaseAction::Init(AFretFinger *fretFinger, int8 string, float releaseDuration)
{
	mFretFinger = fretFinger;
	mString = string;
	mReleaseDuration = releaseDuration;
}

void UFretFingerReleaseAction::Execute()
{
	mFretFinger->GetAnimator()->ReleaseString(mString, mReleaseDuration);
}


