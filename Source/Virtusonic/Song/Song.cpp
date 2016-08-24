// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "Song.h"

/*
 * Returns the song tempo in quarter notes per minute.
 */
void USong::SetTempo(int32 tempo)
{
	mTempo = tempo;
}

/*
* Set the song tempo in quarter notes per minute.
*/
int32 USong::GetTempo()
{
	return mTempo;
}

/*
* Returns the number of ticks per quarter note.
*/
void USong::SetTicksPerQuarter(int32 ticksPerQuarter)
{
	mTicksPerQuarter = ticksPerQuarter;
}

/*
* Sets the number of ticks per quarter note.
*/
int32 USong::GetTicksPerQuarter()
{
	return mTicksPerQuarter;
}

/*
 * Adds a new empty track with the given name to the track list.
 */
void USong::AddTrack(FString trackName)
{
	FSongTrack track;
	track.Name = trackName;

	mTracks.Add(trackName, track);
}

/*
 * Starts a new note to be added to the currently active track. 
 * Requires EndNote() to be called in order to actually add the note to the track.
 */
void USong::StartNote(int32 startTick, int32 pitch, int32 velocity)
{
	USongNote* note = NewObject<USongNote>();
	note->Init(startTick, -1, pitch, velocity);
	mStartedNotes.Add(note);
}

/*
 * Fills in the remaining parameters for the note and adds it to the track with the given name.
 */
void USong::EndNote(FString trackName, int32 endTick, int32 pitch, int32 velocity)
{
	for (int i = 0; i < mStartedNotes.Num(); i++)
	{
		if (mStartedNotes[i]->GetPitch() == pitch && mStartedNotes[i]->GetStartTick() < endTick)
		{
			mStartedNotes[i]->SetEndTick(endTick);
			AddNoteToTrack(trackName, mStartedNotes[i]);
			mStartedNotes.RemoveAt(i);
			return;
		}
	}
}

/*
 * Returns the list of track names in the loaded song.
 */
TArray<FString> USong::TrackNames()
{
	TArray<FString> keys;
	for (auto it = mTracks.CreateIterator(); it; ++it)
	{
		keys.Add(it->Value.Name);
	}
	return keys;
}

/*
 * Generates the song timeline for the given instrument.
 */
void USong::GenerateTimeline(AInstrument* instrument)
{
	FSongTrack track = GetTrack(instrument->Name());

	// The tempo / ticks per quarter are required for calculating the animation lengths
	instrument->SetSongInfo(mTempo, mTicksPerQuarter);

	// Make sure the notes are sorted by the start tick
	SortNotesByStart(track);

	// Generate the timeline and sort it again (some actions are added retroactively so they are probably out of place)
	USongTimeline* timeline = NewObject<USongTimeline>();
	timeline->AddActions(instrument->GenerateActions(track.Notes));
	timeline->SortByTick();

	mTimelines.Add(timeline);
}

/*
 * Returns the timeline actions at the given tick.
 */
TArray<UBaseTimelineAction*> USong::GetTimelineActions(int32 tick)
{
	TArray<UBaseTimelineAction*> actions;

	for (int32 i = 0; i < mTimelines.Num(); i++)
	{
		actions.Append(mTimelines[i]->GetActionsAtTick(tick));
	}

	return actions;
}

/*
* Adds the given note to the track with the given name.
*/
void USong::AddNoteToTrack(FString trackName, USongNote* note)
{
	mTracks[trackName].Notes.Add(note);
	UE_LOG(VirtusonicLog, Log, TEXT("Added note to %s: %d %d %d %d"), *trackName, note->GetStartTick(), note->GetEndTick(), note->GetPitch(), note->GetVelocity());
}

/*
 * Returns the track with the given name if it exists. If not, returns an empty song track.
 */
FSongTrack USong::GetTrack(FString trackName)
{
	for (auto it = mTracks.CreateIterator(); it; ++it)
	{
		if (trackName.Equals(it->Key))
		{
			return it->Value;
		}
	}

	UE_LOG(VirtusonicLog, Log, TEXT("Could not find track: %s"), *trackName);
	return FSongTrack();
}

/*
 * Sorts the track notes by the start tick.
 */
void USong::SortNotesByStart(FSongTrack &track)
{
	track.Notes.StableSort(NoteSortPredicate);
}




