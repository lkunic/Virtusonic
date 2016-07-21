// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Pick.h"


// Sets default values
APick::APick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APick::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

