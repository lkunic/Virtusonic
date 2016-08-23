// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongTimeline.h"

/*
 * Adds the given list of actions to the timeline.
 */
void USongTimeline::AddActions(const TArray<UBaseTimelineAction*> &actions)
{
	mActionTimeline.Append(actions);
}

/*
 * Sorts the timeline actions by tick.
 */
void USongTimeline::SortByTick()
{
	mActionTimeline.StableSort(TimelineSortPredicate);
}

/*
 * Returns the list of actions at the given tick.
 */
TArray<UBaseTimelineAction*> USongTimeline::GetActionsAtTick(int32 tick)
{
	TArray<UBaseTimelineAction*> actions;

	for (int32 i = 0; i < mActionTimeline.Num(); i++)
	{
		if (mActionTimeline[i]->Tick == tick)
		{
			actions.Add(mActionTimeline[i]);
		}
	}

	return actions;
}



