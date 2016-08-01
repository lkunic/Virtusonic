// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickRestAction.h"

/*
 * Initializes the action with the given pick reference and the current string position.
 */
void UPickRestAction::Init(APick* pick, TCHAR fromString)
{
	_pick = pick;
	_animation = _pick->GetAnimator()->GetAnimationSequence(EPickAnimations::PickReadyXToRest, fromString);
}

/*
 * Returns the animation length.
 */
float UPickRestAction::GetAnimationLength()
{
	return _animation->SequenceLength / _animation->RateScale;
}

/*
 * Starts the pick rest animation.
 */
void UPickRestAction::Execute()
{
	_pick->GetSkeletalMeshComponent()->Stop();
	_pick->GetSkeletalMeshComponent()->PlayAnimation(_animation, false);
}


