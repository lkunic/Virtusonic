// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioSource.h"


// Sets default values
AAudioSource::AAudioSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAudioSource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAudioSource::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

