// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerReleaseAction.h"

void UFretFingerReleaseAction::Init(AFretFinger *fretFinger, int8 string, int32 noteStartTick, float releaseDuration)
{
	mFretFinger = fretFinger;
	mString = string;
	mNoteStartTick = noteStartTick;
	mReleaseDuration = releaseDuration;
}

void UFretFingerReleaseAction::Execute()
{
	mFretFinger->GetAnimator()->ReleaseString(mString, mNoteStartTick, mReleaseDuration);
}


