// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongTimeline.h"

/* 
 * Initializes the timeline.
 */
void USongTimeline::Init()
{
	_nextAction = 0;
}

/*
 * Adds the given list of actions to the timeline.
 */
void USongTimeline::AddActions(TArray<UBaseTimelineAction*> actions)
{
	_actionTimeline.Append(actions);
}

/*
 * Sorts the timeline actions by tick.
 */
void USongTimeline::SortByTick()
{
	_actionTimeline.StableSort(TimelineSortPredicate);
}

/*
 * Returns the list of actions at the given tick.
 */
TArray<UBaseTimelineAction*> USongTimeline::GetActionsAtTick(int32 tick)
{
	TArray<UBaseTimelineAction*> actions;

	while (_nextAction < _actionTimeline.Num() && _actionTimeline[_nextAction]->Tick <= tick)
	{
		actions.Add(_actionTimeline[_nextAction]);
		_nextAction++;
	}

	return actions;
}



