// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "BaseTimelineAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UBaseTimelineAction : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Execute();

	int32 Tick;
};
