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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Song")
	void SongTick();

	UFUNCTION(BlueprintCallable, Category = "Song")
	void GenerateTimelines();

	UFUNCTION(BlueprintCallable, Category = "Song")
	float GetTimePerTick();

	UFUNCTION(BlueprintCallable, Category = "Song")
	void SetInstruments(TArray<AInstrument*> instruments);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Song")
	FString _songName = "CMajor";

private:
	USong* _song;
	int32 currentTick;

	TArray<AInstrument*> _instruments;
};
