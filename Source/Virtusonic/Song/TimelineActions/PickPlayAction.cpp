// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickPlayAction.h"

void UPickPlayAction::Init(APick* pick, TCHAR stringRoot)
{
	_pick = pick;
	_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickX, stringRoot);
}

float UPickPlayAction::GetAnimationLength()
{
	return _animation->SequenceLength / _animation->RateScale;
}

void UPickPlayAction::Execute()
{
	_pick->GetSkeletalMeshComponent()->Stop();
	_pick->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
	UE_LOG(VirtusonicLog, Log, TEXT("Playing pick %s"), *(_pick->GetName()));
}



