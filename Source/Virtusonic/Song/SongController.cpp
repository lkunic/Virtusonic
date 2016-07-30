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

	UE_LOG(VirtusonicLog, Log, TEXT("Loading song: %s.mid"), *_songName);
	
	_song = NewObject<USong>();
	_song->AddToRoot();
	USongLoader::ParseMidiFile(FPaths::GameContentDir().Append("\\Songs\\").Append(*_songName).Append(".mid"), _song);

	currentTick = 0;
}

void ASongController::SongTick()
{
	TArray<UBaseTimelineAction*> actions = _song->GetTimelineActions(currentTick);

	if (actions.Num() != 0)
	{
		for (auto it = actions.CreateIterator(); it; it++)
		{
			(*it)->Execute();
		}
	}

	currentTick++;
}

void ASongController::GenerateTimelines()
{
	TArray<FString> trackNames = _song->TrackNames();
	for (int i = 0; i < _instruments.Num(); i++)
	{
		if (trackNames.Contains(_instruments[i]->Name()))
		{
			UE_LOG(VirtusonicLog, Log, TEXT("Generating timeline for track: %s"), *trackNames[i]);
			
			_song->GenerateTimeline(_instruments[i]);
		}
	}
}

float ASongController::GetTimePerTick()
{
	return 60.0f / (_song->GetTempo() * _song->GetTicksPerQuarter());
}

void ASongController::SetInstruments(TArray<AInstrument*> instruments)
{
	_instruments = instruments;
}

