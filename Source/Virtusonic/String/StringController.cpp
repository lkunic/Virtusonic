// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringController.h"


// Sets default values
AStringController::AStringController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStringController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStringController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

