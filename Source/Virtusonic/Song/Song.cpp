// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Song.h"


void USong::SetTempo(int32 tempo)
{
	_tempo = tempo;
}

int32 USong::GetTempo()
{
	return _tempo;
}

void USong::SetTicksPerQuarter(int32 ticksPerQuarter)
{
	_ticksPerQuarter = ticksPerQuarter;
}

int32 USong::GetTicksPerQuarter()
{
	return _ticksPerQuarter;
}

/*
 * Adds a new empty track with the given name to the track list.
 */
void USong::AddTrack(FString trackName)
{
	SongTrack track;
	track.name = trackName;

	_tracks.Add(trackName, track);
}

/*
 * Starts a new note to be added to the currently active track. 
 * Requires EndNote() to be called in order to actually add the note to the track.
 */
void USong::StartNote(int32 startTick, int32 pitch, int32 velocity)
{
	USongNote* note = NewObject<USongNote>();
	note->Init(startTick, -1, pitch, velocity);
	_startedNotes.Add(note);
}

/*
 * Fills in the remaining parameters for the note and adds it to the track with the given name.
 */
void USong::EndNote(FString trackName, int32 endTick, int32 pitch, int32 velocity)
{
	for (int i = 0; i < _startedNotes.Num(); i++)
	{
		if (_startedNotes[i]->GetPitch() == pitch && _startedNotes[i]->GetStartTick() < endTick)
		{
			_startedNotes[i]->SetEndTick(endTick);
			AddNoteToTrack(trackName, _startedNotes[i]);
			_startedNotes.RemoveAt(i);
			return;
		}
	}
}

TArray<FString> USong::TrackNames()
{
	TArray<FString> keys;
	for (auto it = _tracks.CreateIterator(); it; ++it)
	{
		keys.Add(it->Value.name);
	}
	return keys;
}

void USong::GenerateTimeline(AInstrument* instrument)
{
	SongTrack track = GetTrack(instrument->Name());
	USongTimeline* timeline = NewObject<USongTimeline>();

	for (int i = 0, n = track.notes.Num(); i < n; i++)
	{
		timeline->AddActions(instrument->GenerateActions(track.notes[i]));
	}

	timeline->SortByTick();

	_timelines.Add(instrument->Name(), timeline);
}

TArray<UBaseTimelineAction*> USong::GetTimelineActions(int32 tick)
{
	TArray<UBaseTimelineAction*> actions;

	for (auto it = _timelines.CreateIterator(); it; ++it)
	{
		actions.Append(it->Value->GetActionsAtTick(tick));
	}

	return actions;
}

/*
 * Returns the track with the given name if it exists. If not, returns an empty song track.
 */
SongTrack USong::GetTrack(FString trackName)
{
	for (auto it = _tracks.CreateIterator(); it; ++it)
	{
		if (trackName.Equals(it->Key))
		{
			return it->Value;
		}
	}

	UE_LOG(VirtusonicLog, Log, TEXT("Could not find track: %s"), *trackName);
	return SongTrack();
}

/*
 * Adds the given note to the track with the given name.
 */
void USong::AddNoteToTrack(FString trackName, USongNote* note)
{
	_tracks[trackName].notes.Add(note);
	UE_LOG(VirtusonicLog, Log, TEXT("Added note to %s: %d %d %d %d"), *trackName, note->GetStartTick(), note->GetEndTick(), note->GetPitch(), note->GetVelocity());
}



