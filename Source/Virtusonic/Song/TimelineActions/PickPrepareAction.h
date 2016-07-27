// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "PickPrepareAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UPickPrepareAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	virtual void Execute() override;
};
