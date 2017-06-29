// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "SongNote.h"
#include "SongTimeline.h"
#include "Instrument/Instrument.h"
#include "Song/TimelineActions/General/SongFinishedAction.h"
#include "VirtusonicGameState.h"

#include "Song.generated.h"

/*
 * The song track structure, contains the track name (used for mapping to instruments) and a list of notes.
 */
USTRUCT()
struct VIRTUSONIC_API FSongTrack {
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<USongNote*> Notes;
};

/*
 * The predicate used for sorting the notes by start tick.
 */
inline static bool NoteSortPredicate(const USongNote& act1, const USongNote& act2)
{
	return act1.StartTick < act2.StartTick;
}

/**
 * Container class for the song. Used to generate a song timeline with all actions required for playing the song.
 */
UCLASS()
class VIRTUSONIC_API USong : public UObject
{
	GENERATED_BODY()
	
public:
	// Getters and setters
	void SetTempo(int32 tempo);
	int32 GetTempo();
	void SetTicksPerQuarter(int32 ticksPerQuarter);
	int32 GetTicksPerQuarter();

	// Generating the tracks and timelines
	void AddTrack(FString trackName);
	void StartNote(int32 startTick, int32 pitch, int32 velocity);
	void EndNote(FString trackName, int32 endTick, int32 pitch, int32 velocity);
	void GenerateTimeline(AInstrument* instrument);
	void GenerateControlTimeline(AVirtusonicGameState* gameState);

	TArray<FString> TrackNames();
	TArray<UBaseTimelineAction*> GetTimelineActions(int32 tick);

private:
	// Private helper functions
	void AddNoteToTrack(FString trackName, USongNote *note);
	FSongTrack GetTrack(FString trackName);
	void SortNotesByStart(FSongTrack &track);

	int32 mTempo;
	int32 mTicksPerQuarter;
	TMap<FString, FSongTrack> mTracks;
	TArray<USongNote*> mStartedNotes;
	int32 mLastTick = 0;

	UPROPERTY()
	TArray<USongTimeline*> mTimelines;
};
