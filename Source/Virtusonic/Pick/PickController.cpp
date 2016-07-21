// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickController.h"


// Sets default values
APickController::APickController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

