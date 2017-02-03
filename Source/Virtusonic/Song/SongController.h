// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "SongLoader.h"
#include "Instrument/Instrument.h"

#include "SongController.generated.h"

UCLASS()
class VIRTUSONIC_API ASongController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASongController();

	UFUNCTION(BlueprintCallable, Category = "Song")
	void LoadSong();

	// This function is periodically called by the blueprint timer
	UFUNCTION(BlueprintCallable, Category = "Song")
	void SongTick();

	// Generates the timelines for the list of available isntruments
	UFUNCTION(BlueprintCallable, Category = "Song")
	void GenerateTimelines();

	// Calculates the delay between consecutive ticks
	UFUNCTION(BlueprintCallable, Category = "Song")
	float GetTimePerTick();

	// Sets the list of available isntruments
	UFUNCTION(BlueprintCallable, Category = "Song")
	void SetInstruments(TArray<AInstrument*> instruments);

	// The song name (used for loading the song file)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song")
	FString SongName = "CMajor";

private:
	UPROPERTY()
	USong *mSong;

	int32 currentTick;

	TArray<AInstrument*> mInstruments;
};
