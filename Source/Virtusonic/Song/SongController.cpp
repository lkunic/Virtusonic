// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongController.h"


// Sets default values
ASongController::ASongController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASongController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASongController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

