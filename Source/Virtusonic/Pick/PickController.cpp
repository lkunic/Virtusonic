// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickController.h"


// Sets default values for this component's properties
UPickController::UPickController()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

/*
 * Returns the number of string.
 */
int32 UPickController::GetPickCount()
{
	return _picks.Num();
}

/*
 * Returns the string with the given index.
 */
APick* UPickController::GetPick(int32 index)
{
	return _picks[index];
}

/*
 * Blueprint function for adding a string to this controller.
 */
void UPickController::AddPick(APick* pick)
{
	_picks.Add(pick);
}
