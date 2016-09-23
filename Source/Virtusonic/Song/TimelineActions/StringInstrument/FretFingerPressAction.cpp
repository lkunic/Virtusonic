// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerPressAction.h"

void UFretFingerPressAction::Init(AFretFinger *fretFinger, int8 string, int32 noteStartTick, float pressDuration)
{
	mFretFinger = fretFinger;
	mString = string;
	mNoteStartTick = noteStartTick;
	mPressDuration = pressDuration;
}

void UFretFingerPressAction::Execute()
{
	mFretFinger->GetAnimator()->PressString(mString, mNoteStartTick, mPressDuration);
}