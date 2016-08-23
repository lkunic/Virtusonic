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
	static bool ParseMidiFile(const FString &filename, USong &outSong);

private:
	static bool GetHeaderData(const TArray<uint8> &sar, int32 &index, int32 &outTrackCount, int32 &outTicksPerQuarter);
	static bool GetTrackContent(const TArray<uint8> &sar, int32 &index , TrackEvents &outTrackEvents, const int32 ticksPerQuarter);
	static bool ReadMidiEvent(const TArray<uint8> &sar, int32 &index, TArray<int32> &outEventData, int32 &runningCommand, const int32 absTicks);

	static int32 GetVarLength(const TArray<uint8> &sar, int32 &index);
	static int32 GetSongTempo(const TArray<int32> &tempoMetaEvent);
	static FString GetTrackName(const TArray<int32> &trackNameMetaEvent);
};
