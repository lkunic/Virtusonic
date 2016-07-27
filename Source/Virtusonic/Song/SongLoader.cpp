// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongLoader.h"

bool USongLoader::ParseMidiFile(FString filename, USong* song)
{
	TArray<uint8> sar;
	int32 index = 0;

	int32 trackCount;
	int32 ticksPerQuarter;
	FString trackName;
	TArray<USongNote*> startedNotes;

	bool isLoadSuccessful = FFileHelper::LoadFileToArray(sar, *filename);

	if (!isLoadSuccessful)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("Could not load file: %s"), *filename);
	}

	if (sar.Num() == 0)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("File empty: %d"), *filename);
		return false;
	}

	if (!GetHeaderData(sar, &index, &trackCount, &ticksPerQuarter))
	{
		// There was an error reading the file header.
		return false;
	}

	song->SetTicksPerQuarter(ticksPerQuarter);

	UE_LOG(VirtusonicLog, Log, TEXT("Track count: %d\n"), trackCount);
	UE_LOG(VirtusonicLog, Log, TEXT("Ticks per quarter: %d\n"), ticksPerQuarter);

	// Iterate through the tracks.
	// Track 0 should contain the time signature metadata, which defaults to 4/4 and should remain the same
	// for all MIDI files, so this can safely be ignored for this app.
	// Track 1 contains the tempo, which is important and needs to be parsed. 
	// The remaining tracks contain the notes for the song, one track per instrument. The first event is a meta event
	// containing the track name, which is used to determine the instrument playing the track.
	for (int trackId = 0; trackId < trackCount; trackId++)
	{
		TrackEvents trackEvents;

		if (!GetTrackContent(sar, &index, &trackEvents))
		{
			return false;
		}

		if (trackId == 1)
		{
			// The second track (index 1) should contain the tempo metadata
			if (trackEvents[0][1] == 0xff && trackEvents[0][2] == 0x51)
			{
				song->SetTempo(GetSongTempo(trackEvents[0]));
				UE_LOG(VirtusonicLog, Log, TEXT("Tempo: %d\n"), song->GetTempo());
			}
			else
			{
				UE_LOG(VirtusonicError, Warning, TEXT("Invalid metadata. Expected tempo data on track 2."));
				return false;
			}
		}

		if (trackId >= 2)
		{
			// Each song track should contain the track title metadata
			if (trackEvents[0][1] == 0xff && trackEvents[0][2] == 0x03)
			{
				trackName = GetTrackName(trackEvents[0]);
				song->AddTrack(trackName);

				UE_LOG(VirtusonicLog, Log, TEXT("Added track %s"), *trackName);

				// The rest of the events in the track are notes
				for (int32 i = 1; i < trackEvents.Num(); i++)
				{
					// If the note event command starts with 0x9, that means that a note is starting
					if ((trackEvents[i][1] & 0xf0) == 0x90)
					{
						song->StartNote(trackEvents[i][0], trackEvents[i][2], trackEvents[i][3]);
					}
					// If the note event command starts with 0x8, the note is ending 
					else if ((trackEvents[i][1] & 0xf0) == 0x80)
					{
						song->EndNote(trackName, trackEvents[i][0], trackEvents[i][2], trackEvents[i][3]);
					}
				}
			}
			else
			{
				UE_LOG(VirtusonicError, Warning, TEXT("Invalid metadata. Expected track name data on track %d."), trackId);
				return false;
			}
		}
	}

	return true;
}

/*
* Parses the header in order to retrieve the track count and the number of ticks per quarter note.
* The track count should be at least 3 (the first two tracks contain metadata and every following track contains notes for an instrument).
*/
bool USongLoader::GetHeaderData(TArray<uint8> sar, int32* index, int32* trackCount, int32* ticksPerQuarter)
{
	char buffer[6] = { 0 };
	int32 headerSize;
	int32 midiFormat;

	// The header should start with 'MThd'
	buffer[0] = sar[(*index)++];
	buffer[1] = sar[(*index)++];
	buffer[2] = sar[(*index)++];
	buffer[3] = sar[(*index)++];
	buffer[4] = '\0';
	if (strcmp((char*)buffer, MIDI_HEADER_LABEL) != 0)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("Given file does not have a MIDI header. File must begin with 'MThd'."));
		return false;
	}

	// The following 4 bytes contain the header length, which should always equal 6 bytes
	buffer[0] = sar[(*index)++];
	buffer[1] = sar[(*index)++];
	buffer[2] = sar[(*index)++];
	buffer[3] = sar[(*index)++];
	headerSize = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
	if (headerSize != 0x00000006)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("Header length invalid. Should be 6 and is %d."), headerSize);
		return false;
	}

	// The 6 data bytes contain the midi format (2 bytes), track count (2 bytes, this is what we need) and
	// the delta-time division units (2 bytes), which should default to 60
	buffer[0] = sar[(*index)++];
	buffer[1] = sar[(*index)++];
	buffer[2] = sar[(*index)++];
	buffer[3] = sar[(*index)++];
	buffer[4] = sar[(*index)++];
	buffer[5] = sar[(*index)++];
	midiFormat = (buffer[0] << 8) | buffer[1];
	if (midiFormat != 0x0001)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("Midi format invalid. Should be 1 (multitrack) and is %d."), midiFormat);
		return false;
	}

	*trackCount = (buffer[2] << 8) | buffer[3];
	*ticksPerQuarter = ((buffer[4] << 8) | buffer[5]) / 8;

	return true;
}

/*
* Parses the track bytes and stores the track events in a vector of byte vectors.
*/
bool USongLoader::GetTrackContent(TArray<uint8> sar, int32* index, TrackEvents* trackEvents)
{
	unsigned char buffer[5] = { 0 };

	int32 trackSize; // This should be the byte count for the track, but can be wrong
	int32 absTicks; // Elapsed time in ticks for each event (event timings are stored relative to the previous event)
	int32 varlength;

	TArray<int32> eventData; // Data for one event
	int32 runningCommand; // Event command labels can be omitted, so this is used to track the running event

						  // The track data should start with 'MTrk'
	buffer[0] = sar[(*index)++];
	buffer[1] = sar[(*index)++];
	buffer[2] = sar[(*index)++];
	buffer[3] = sar[(*index)++];
	buffer[4] = '\0';
	if (strcmp((char*)buffer, MIDI_TRACK_LABEL) != 0)
	{
		UE_LOG(VirtusonicError, Warning, TEXT("Given file does not have any tracks."));
		return false;
	}

	// The track size is read from the following 4 bytes, but this can be inacurate so the delimiter event 'FF 2F' is used
	// to determine the end of the track
	buffer[0] = sar[(*index)++];
	buffer[1] = sar[(*index)++];
	buffer[2] = sar[(*index)++];
	buffer[3] = sar[(*index)++];
	trackSize = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

	trackEvents->Reserve(trackSize / 2);
	trackEvents->Empty();

	absTicks = 0;

	while (!((*index) >= sar.Num()))
	{
		// Update the tick count at the current event
		varlength = GetVarLength(sar, index);
		absTicks += varlength / 8;

		// Read the event data and add it to the list
		if (ReadMidiEvent(sar, index, &eventData, &runningCommand, absTicks))
		{
			trackEvents->Add(eventData);
		}

		// Check for the delimiter event
		if (eventData[1] == 0xff && eventData[2] == 0x2f)
		{
			return true;
		}
	}

	return false;
}

/*
* The MIDI events can be meta, system, or channel events. Meta and system events have a command byte of the form 0xfn
* and contain various metadata such as tempo, time signature, or track names.
* The channel events contain note data (0x9n indicates start of a note, 0x8n is the note ending) and some other channel
* settings which can be ignored.
*/
bool USongLoader::ReadMidiEvent(TArray<uint8> sar, int32* index, TArray<int32>* eventData, int32* runningCommand, int32 absTicks)
{
	int32 byte;
	int32 metaLength;
	bool running;

	// Add the tick time to the event
	eventData->Empty();
	eventData->Add(absTicks);

	byte = sar[(*index)++];
	if (byte < 0x80)
	{
		// If the byte is less than 0x80, this means that there is no command byte, so the running command is used
		running = true;
	}
	else
	{
		// The byte is a new command so the running command is updated
		running = false;
		*runningCommand = byte;
	}

	// Add the command byte to the event
	eventData->Add(*runningCommand);
	if (running)
	{
		eventData->Add(byte);
	}

	// Check the command byte type (the 4 more significant bits)
	switch (*runningCommand & 0xf0)
	{
	case 0x80:
	case 0x90:
		// 0x8n and 0x9n indicate notes and contain 2 data bytes, so these are saved
		// True return value indicates that the event needs to be added to the list
		byte = sar[(*index)++];
		eventData->Add(byte);
		if (!running)
		{
			byte = sar[(*index)++];
			eventData->Add(byte);
		}
		return true;

		// These event types aren't relevant for the app, so they are ignored (the bytes still need to be read though)
	case 0xA0:
	case 0xB0:
	case 0xE0:
		byte = sar[(*index)++];
	case 0xC0:
	case 0xD0:
		if (!running)
		{
			byte = sar[(*index)++];
		}
		return false;

		// 0xFX events are treated separately depending on the lower 4 bits
	case 0xF0:
		switch (*runningCommand)
		{
		case 0xff:
			// 0xFF indicates meta events which contain important data, so they need to be added to the list
			if (!running)
			{
				byte = sar[(*index)++];
				eventData->Add(byte);
			}

			// The length of the event varies based on the command
			metaLength = sar[(*index)++];
			eventData->Add(metaLength);
			for (int32 i = 0; i < metaLength; i++)
			{
				byte = sar[(*index)++];
				eventData->Add(byte);
			}
			return true;

		case 0xf7:
		case 0xf0:
			// The system meta events aren't important for the app, so they can also be ignored
			metaLength = GetVarLength(sar, index);
			for (int32 i = 0; i < metaLength; i++)
			{
				byte = sar[(*index)++];
			}
			return false;
		}
		break;
	default:
		UE_LOG(VirtusonicError, Warning, TEXT("Error reading MIDI event. Found %x command byte."), *runningCommand);
		return false;
	}

	return true;
}

/*
* The variable length values are used to express event length quantities. In order to prevent unnecessary allocation
* of memory, this system is used to express values up to a 4-byte integer using 1-4 bytes.
* The lower 7 bits of each byte are used to store the value, and the most significant bit indicates whether the value
* is continued in the next byte. If the bit is set, there is another byte containing the continuation of the value,
* and if it is unset, the byte is the last one in the sequence.
*
* e.g. '0x81 0x7f' would indicate the value of 0xff, and '0x82 0x80 0x00' would be 0x8000.
*/
int32 USongLoader::GetVarLength(TArray<uint8> sar, int32* index)
{
	long length = 0;
	int32 byte;

	int32 i = 4;
	while (i--)
	{
		byte = sar[(*index)++];
		length = length << 7 | (byte & 0x7f);
		if (byte < 0x80)
		{
			break;
		}
	}

	return length;
}

/*
* The tempo is set by a meta event of length 7. First byte is the delta tick which should be set to 0x00.
* The next two bytes indicate a meta event for setting the tempo (0xff 0x51). Then follows the meta event
* length of 0x03, and bytes 4, 5, 6 contain the tempo expressed in microseconds per beat.
* Therefore, the tempo is calculated by dividing the number of microseconds in a minute (60kk) and the
* microseconds-per-beat value given by the meta event.
*/
int32 USongLoader::GetSongTempo(TArray<int32> tempoMetaEvent)
{
	long microseconds = (tempoMetaEvent[4] << 16) | (tempoMetaEvent[5] << 8) | tempoMetaEvent[6];

	int32 tempo = 60000000 / microseconds;

	// Fix the off-by-one error caused when dividing by 3.
	if ((tempo - 9) % 10 == 0)
	{
		tempo += 1;
	}

	return tempo;
}

/*
* The track name is set by a meta event. The first byte is the delta tick which should be set to 0x00.
* The next two bytes indicate a meta event for setting the track name (0xff 0x03). Then follows the
* length of the track name, and then 'length' amount of bytes containing the actual name.
*/
FString USongLoader::GetTrackName(TArray<int32> trackNameMetaEvent)
{
	int32 nameLength = trackNameMetaEvent[3];
	char* name = (char*)malloc((nameLength + 1) * sizeof(char));

	for (int i = 0; i < nameLength; i++)
	{
		name[i] = trackNameMetaEvent[i + 4];
	}

	name[nameLength] = '\0';

	return name;
}


