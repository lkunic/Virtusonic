// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "TimelineActions/BaseTimelineAction.h"

#include "SongTimeline.generated.h"

/*
* The predicate used for sorting the timeline actions by tick.
*/
inline static bool TimelineSortPredicate(const UBaseTimelineAction& act1, const UBaseTimelineAction& act2)
{
	return act1.Tick < act2.Tick;
}

/**
 * The song timeline which acts as a container for the actions that need to perform for a song track.
 */
UCLASS()
class VIRTUSONIC_API USongTimeline : public UObject
{
	GENERATED_BODY()
	
public:

	void AddActions(const TArray<UBaseTimelineAction*> &actions);
	void SortByTick();

	TArray<UBaseTimelineAction*> GetActionsAtTick(int32 tick);

private:
	UPROPERTY()
	TArray<UBaseTimelineAction*> mActionTimeline;
};
