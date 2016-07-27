// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "TimelineActions/BaseTimelineAction.h"

#include "SongTimeline.generated.h"

inline static bool TimelineSortPredicate(const UBaseTimelineAction& act1, const UBaseTimelineAction& act2)
{
	return act1.Tick < act2.Tick;
}

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API USongTimeline : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	void AddActions(TArray<UBaseTimelineAction*> actions);
	void SortByTick();

	TArray<UBaseTimelineAction*> GetActionsAtTick(int32 tick);

private:
	TArray<UBaseTimelineAction*> _actionTimeline;
	int32 _nextAction;
};
