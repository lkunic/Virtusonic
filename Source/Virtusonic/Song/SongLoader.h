// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "Song.h"

#include "SongLoader.generated.h"

#define MIDI_HEADER_LENGTH 14
#define MIDI_HEADER_LABEL "MThd"
#define MIDI_TRACK_LABEL "MTrk"

typedef TArray<TArray<int32>> TrackEvents;

/**
 * Utility class for loading MIDI files with songs.
 */
UCLASS()
class VIRTUSONIC_API USongLoader : public UObject
{
	GENERATED_BODY()

public:
	static bool ParseMidiFile(FString filename, USong* song);

private:
	static bool GetHeaderData(TArray<uint8> sar, int32* index, int32* trackCount, int32* ticksPerQuarter);
	static bool GetTrackContent(TArray<uint8> sar, int32* index , TrackEvents* trackEvents, int32 ticksPerQuarter);
	static bool ReadMidiEvent(TArray<uint8> sar, int32* index, TArray<int32>* eventData, int32* runningCommand, int32 absTicks);

	static int32 GetVarLength(TArray<uint8> sar, int32* index);
	static int32 GetSongTempo(TArray<int32> tempoMetaEvent);
	static FString GetTrackName(TArray<int32> trackNameMetaEvent);
};
