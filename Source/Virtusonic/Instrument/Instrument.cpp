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

void AInstrument::SetSongInfo(int32 tempo, int32 ticksPerQuarter)
{
	_tempo = tempo;
	_ticksPerQuarter = ticksPerQuarter;
}

FString AInstrument::Name()
{
	return "Instrument";
}

TArray<UBaseTimelineAction*> AInstrument::GenerateActions(TArray<USongNote*> notes)
{
	return TArray<UBaseTimelineAction*>();
}

