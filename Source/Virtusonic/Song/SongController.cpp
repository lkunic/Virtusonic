// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongController.h"


// Sets default values
ASongController::ASongController()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASongController::BeginPlay()
{
	Super::BeginPlay();

	if (SongName.IsEmpty())
	{
		SongName = "CMajor";
	}
	UE_LOG(VirtusonicLog, Log, TEXT("Loading song: %s.mid"), *SongName);
	
	// Loads the MIDI file containing the song from the content folder
	mSong = NewObject<USong>();
	mSong->AddToRoot();
	USongLoader::ParseMidiFile(FPaths::GameContentDir().Append("Songs/").Append(*SongName).Append(".mid"), *mSong);

	// Initializes the tick counter
	currentTick = 0;
}

/*
 * The event function which gets periodically called by the timer for each tick.
 * Goes through the instrument timelines and executes all actions at the current tick.
 */
void ASongController::SongTick()
{
	TArray<UBaseTimelineAction*> actions = mSong->GetTimelineActions(currentTick);

	if (actions.Num() != 0)
	{
		for (auto it = actions.CreateIterator(); it; it++)
		{
			(*it)->Execute();
		}
	}

	currentTick++;
}

/*
 * Generates the timelines for all tracks/instruments.
 */
void ASongController::GenerateTimelines()
{
	TArray<FString> trackNames = mSong->TrackNames();
	for (int i = 0; i < mInstruments.Num(); i++)
	{
		if (trackNames.Contains(mInstruments[i]->Name()))
		{
			UE_LOG(VirtusonicLog, Log, TEXT("Generating timeline for track: %s"), *trackNames[i]);
			
			mSong->GenerateTimeline(mInstruments[i]);
		}
	}
}

/*
 * Calculates the time delay in seconds between consecutive ticks.
 */
float ASongController::GetTimePerTick()
{
	return 60.0f / (mSong->GetTempo() * mSong->GetTicksPerQuarter());
}

/*
 * Sets the available instruments.
 */
void ASongController::SetInstruments(TArray<AInstrument*> instruments)
{
	mInstruments = instruments;
}

