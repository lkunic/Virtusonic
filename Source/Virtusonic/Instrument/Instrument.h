// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "Song/SongNote.h"
#include "Song/TimelineActions/BaseTimelineAction.h"

#include "Instrument.generated.h"

/**
 * Base 'abstract' instrument actor class, defines the interface for all instrument actors.
 */
UCLASS()
class VIRTUSONIC_API AInstrument : public AActor
{
	GENERATED_BODY()
	
public:	
	AInstrument();

	void SetSongInfo(int32 tempo, int32 ticksPerQuarter);
	
	// The instrument name, used for mapping to MIDI tracks
	virtual FString Name();
	virtual TArray<UBaseTimelineAction*> GenerateActions(TArray<USongNote*> notes);

protected:
	int32 _tempo;
	int32 _ticksPerQuarter;
	int32 _songLength;
};
