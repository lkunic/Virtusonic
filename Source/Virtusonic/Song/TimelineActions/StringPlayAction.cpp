// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringPlayAction.h"

/*
* Initializes the action with the given string reference and the current string position.
*/
void UStringPlayAction::Init(AString *string, TCHAR stringRoot)
{
	mString = string;
	mAnimation = mString->GetAnimator()->GetAnimationSequence(EStringAnimations::StringVibrationUpX, stringRoot);
}

/*
* Starts the string play animation.
*/
void UStringPlayAction::Execute()
{
	mString->GetSkeletalMeshComponent()->Stop();
	mString->GetSkeletalMeshComponent()->PlayAnimation(mAnimation, false);
	//UE_LOG(VirtusonicLog, Log, TEXT("Playing string %s"), *(_string->GetName()));
}





