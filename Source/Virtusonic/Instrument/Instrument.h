// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "Song/SongNote.h"
#include "Song/TimelineActions/BaseTimelineAction.h"

#include "Instrument.generated.h"

/**
 *
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
	

	virtual FString Name();
	virtual TArray<UBaseTimelineAction*> GenerateActions(USongNote* note);
};
