// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringPlayAction.h"

/*
* Initializes the action with the given string reference and the current string position.
*/
void UStringPlayAction::Init(AString* string, TCHAR stringRoot)
{
	_string = string;
	_animation = _string->GetAnimator()->GetAnimationSequence(EStringAnimations::StringVibrationUpX, stringRoot);
}

/*
* Starts the string play animation.
*/
void UStringPlayAction::Execute()
{
	_string->GetSkeletalMeshComponent()->Stop();
	_string->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
	//UE_LOG(VirtusonicLog, Log, TEXT("Playing string %s"), *(_string->GetName()));
}





