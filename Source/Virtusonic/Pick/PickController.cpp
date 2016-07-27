// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickController.h"


// Sets default values for this component's properties
UPickController::UPickController()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPickController::BeginPlay()
{
	Super::BeginPlay();
}

int32 UPickController::GetPickCount()
{
	return _picks.Num();
}

void UPickController::AddPick(APick* pick)
{
	_picks.Add(pick);
}
