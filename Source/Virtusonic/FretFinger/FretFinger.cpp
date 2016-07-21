// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFinger.h"


// Sets default values
AFretFinger::AFretFinger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFretFinger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFretFinger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

