// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerPressAction.h"

void UFretFingerPressAction::Init(AFretFinger *fretFinger, int8 string, float pressDuration)
{
	mFretFinger = fretFinger;
	mString = string;
	mPressDuration = pressDuration;
}

void UFretFingerPressAction::Execute()
{
	mFretFinger->GetAnimator()->PressString(mString, mPressDuration);
}