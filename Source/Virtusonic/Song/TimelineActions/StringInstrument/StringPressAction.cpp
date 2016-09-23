// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringPressAction.h"

void UStringPressAction::Init(AString *string, int8 fret, int32 noteStartTick, float pressDuration)
{
	mString = string;
	mFret = fret;
	mNoteStartTick = noteStartTick;
	mPressDuration = pressDuration;
}

void UStringPressAction::Execute()
{
	mString->GetAnimator()->PressString(mFret, mNoteStartTick, mPressDuration);
}


