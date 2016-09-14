// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Pick/Pick.h"

#include "PickRestAction.generated.h"

/**
 * The song timeline action for returning the pick to the resting position.
 */
UCLASS()
class VIRTUSONIC_API UPickRestAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	// Initializes the action with the given pick reference and the current string position
	void Init(APick *pick, TCHAR fromString);

	float GetAnimationLength();

	virtual void Execute() override;

private:
	UPROPERTY()
	APick *mPick;

	UPROPERTY()
	UAnimSequence* mAnimation;
};
