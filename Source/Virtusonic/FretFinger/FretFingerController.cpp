// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerController.h"


// Sets default values
AFretFingerController::AFretFingerController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFretFingerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFretFingerController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

