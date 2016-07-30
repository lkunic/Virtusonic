// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickPrepareAction.h"

void UPickPrepareAction::Init(APick* pick, TCHAR fromString, TCHAR toString)
{
	_pick = pick;

	if (fromString == 'R')
	{
		_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::RestToPickReadyX, toString);
	}
	else
	{
		_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToPickReadyY, fromString, toString);
	}

	_animationRate = 1.0f;
}

float UPickPrepareAction::GetAnimationLength()
{
	return _animation->SequenceLength / _animation->RateScale;
}

void UPickPrepareAction::SetAnimationRate(float animationRate)
{
	_animationRate = animationRate;
}

void UPickPrepareAction::Execute()
{
	_animation->RateScale = _animationRate;
	_pick->GetSkeletalMeshComponent()->Stop();
	_pick->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
}

