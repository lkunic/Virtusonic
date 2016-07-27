// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Instrument.h"


// Sets default values
AInstrument::AInstrument()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInstrument::BeginPlay()
{
	Super::BeginPlay();
}

FString AInstrument::Name()
{
	return "Instrument";
}

TArray<UBaseTimelineAction*> AInstrument::GenerateActions(USongNote* note)
{
	return TArray<UBaseTimelineAction*>();
}

