// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Instrument.h"


// Sets default values
AInstrument::AInstrument()
{
	PrimaryActorTick.bCanEverTick = false;
}

/*
 * Use to set the basic song info (tempo, ticks per quarter note).
 */
void AInstrument::SetSongInfo(int32 tempo, int32 ticksPerQuarter)
{
	mTempo = tempo;
	mTicksPerQuarter = ticksPerQuarter;
}

/*
 * The instrument name, used for mapping to MIDI tracks.
 */
FString AInstrument::Name()
{
	return "Instrument";
}

/*
 * Generates timeline actions for the given set of notes.
 */
TArray<UBaseTimelineAction*> AInstrument::GenerateActions(const TArray<USongNote*> &notes)
{
	return TArray<UBaseTimelineAction*>();
}

