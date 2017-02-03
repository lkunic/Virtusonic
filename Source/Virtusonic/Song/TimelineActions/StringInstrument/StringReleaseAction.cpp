// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringReleaseAction.h"

void UStringReleaseAction::Init(AString *string, int8 fret, int32 noteStartTick, float releaseDuration)
{
	mString = string;
	mFret = fret;
	mNoteStartTick = noteStartTick;
	mReleaseDuration = releaseDuration;
}

void UStringReleaseAction::Execute()
{
	mString->GetAnimator()->ReleaseString(mFret, mNoteStartTick, mReleaseDuration);
}




