// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Pick/Pick.h"

#include "PickPlayAction.generated.h"

/**
 * The song timeline action for playing the actual picking animation.
 */
UCLASS()
class VIRTUSONIC_API UPickPlayAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	// Initializes the action with the given pick reference and the current string position
	void Init(APick *pick, TCHAR stringRoot);

	float GetAnimationLength();

	virtual void Execute() override;

private:
	UPROPERTY()
	APick *mPick;

	UPROPERTY()
	UAnimSequence *mAnimation;
};
