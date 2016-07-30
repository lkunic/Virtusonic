// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickController.h"


// Sets default values for this component's properties
UPickController::UPickController()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UPickController::GetPickCount()
{
	return _picks.Num();
}

APick* UPickController::GetPick(int32 index)
{
	return _picks[index];
}

void UPickController::AddPick(APick* pick)
{
	_picks.Add(pick);
}
