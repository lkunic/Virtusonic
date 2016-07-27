// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "SongNote.h"
#include "SongTimeline.h"
#include "Instrument/Instrument.h"

#include "Song.generated.h"

typedef struct {
	FString name;
	TArray<USongNote*> notes;
} SongTrack;


/**
 * Container class for the song. Used to generate a song timeline with all actions required for playing the song.
 */
UCLASS()
class VIRTUSONIC_API USong : public UObject
{
	GENERATED_BODY()
	
public:
	void SetTempo(int32 tempo);
	int32 GetTempo();

	void SetTicksPerQuarter(int32 ticksPerQuarter);
	int32 GetTicksPerQuarter();

	void AddTrack(FString trackName);
	void StartNote(int32 startTick, int32 pitch, int32 velocity);
	void EndNote(FString trackName, int32 endTick, int32 pitch, int32 velocity);

	TArray<FString> TrackNames();
	void GenerateTimeline(AInstrument* instrument);

	TArray<UBaseTimelineAction*> GetTimelineActions(int32 tick);

private:
	void AddNoteToTrack(FString trackName, USongNote* note);
	SongTrack GetTrack(FString trackName);

	int32 _tempo;
	int32 _ticksPerQuarter;
	TMap<FString, SongTrack> _tracks;
	TArray<USongNote*> _startedNotes;

	TMap<FString, USongTimeline*> _timelines;
};
