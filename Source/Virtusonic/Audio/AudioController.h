// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "AudioSource.h"

#include "AudioController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UAudioController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioController();

	// Called when the game starts
	virtual void BeginPlay() override;

	int32 GetAudioSourceCount();
	AAudioSource* GetAudioSource(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void AddAudioSource(AAudioSource* audioSource);

private:
	UPROPERTY(EditAnywhere, Category = Audio)
	TArray<AAudioSource*> _audioSources;
};
