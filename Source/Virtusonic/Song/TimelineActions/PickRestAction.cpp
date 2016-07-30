// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickRestAction.h"

void UPickRestAction::Init(APick* pick, TCHAR fromString)
{
	_pick = pick;
	_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToRest, fromString);
}

float UPickRestAction::GetAnimationLength()
{
	return _animation->SequenceLength / _animation->RateScale;
}

void UPickRestAction::Execute()
{
	_pick->GetSkeletalMeshComponent()->Stop();
	_pick->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
}


