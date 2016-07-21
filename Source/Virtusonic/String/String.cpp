// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "String.h"


// Sets default values
AString::AString()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AString::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AString::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

