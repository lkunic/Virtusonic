// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "AudioSource.generated.h"

/**
 * Audio source actor used for playing note sounds.
 */
UCLASS()
class VIRTUSONIC_API AAudioSource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioSource();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetSound(USoundCue *sound, float soundFrequency);

	void PlayNote(double frequency);
	void Stop(double fadeOutDuration = 0.0f);

private:
	// The audio component used for actually playing the sound
	UAudioComponent *mAudioComponent;	

	// The frequency of the base sound, used for modulating the frequency of the sound being played
	float mSoundFrequency;

	// The default volume multiplier for the sound
	float mSoundVolume;
};
