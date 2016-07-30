// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "AudioSource.h"

#include "AudioController.generated.h"


/**
 * Actor component for managing audio sources in an instrument.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UAudioController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioController();

	int32 GetAudioSourceCount();
	AAudioSource* GetAudioSource(int32 index);

	// Blueprint function for adding audio sources at spawn
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void AddAudioSource(AAudioSource* audioSource);

private:
	UPROPERTY(EditAnywhere, Category = Audio)
	TArray<AAudioSource*> _audioSources;
};
