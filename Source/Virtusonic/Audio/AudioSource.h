// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "AudioSource.generated.h"

UCLASS()
class VIRTUSONIC_API AAudioSource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioSource();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayNote(double frequency);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetSound(USoundCue* sound, float soundFrequency);

private:
	UAudioComponent* _audioComponent;	
	float _soundFrequency;
};
