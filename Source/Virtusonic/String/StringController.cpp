// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringController.h"


// Sets default values for this component's properties
UStringController::UStringController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStringController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStringController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

