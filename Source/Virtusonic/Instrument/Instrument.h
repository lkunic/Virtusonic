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
	// Sets default values for this actor's properties
	AInstrument();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetSongInfo(int32 tempo, int32 ticksPerQuarter);
	
	virtual FString Name();
	virtual TArray<UBaseTimelineAction*> GenerateActions(TArray<USongNote*> notes);

protected:
	int32 _tempo;
	int32 _ticksPerQuarter;
	int32 _songLength;
};
