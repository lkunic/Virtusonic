// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioController.h"


// Sets default values
AAudioController::AAudioController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAudioController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAudioController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

