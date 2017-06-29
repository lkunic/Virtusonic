// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "VirtusonicGameState.h"
#include "SongFinishedAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API USongFinishedAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AVirtusonicGameState *gameState);
	virtual void Execute() override;

private:
	AVirtusonicGameState *mGameState;
};
