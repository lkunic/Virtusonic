// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongTimeline.h"

void USongTimeline::Init()
{
	_nextAction = 0;
}

void USongTimeline::AddActions(TArray<UBaseTimelineAction*> actions)
{
	_actionTimeline.Append(actions);
}

void USongTimeline::SortByTick()
{
	_actionTimeline.StableSort(TimelineSortPredicate);
}

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



