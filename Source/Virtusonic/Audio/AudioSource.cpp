// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioSource.h"


// Sets default values
AAudioSource::AAudioSource()
{
	PrimaryActorTick.bCanEverTick = false;

	// Add an audio component to the actor
	mAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSource"));
	RootComponent = mAudioComponent;
}

/*
* Set the sound and the sound base frequency.
*/
void AAudioSource::SetSound(USoundCue *sound, float soundFrequency)
{
	mAudioComponent->Sound = sound;
	mSoundVolume = sound->VolumeMultiplier;
	mSoundFrequency = soundFrequency;
}

/*
 * Plays the stored sound at the given frequency.
 */
void AAudioSource::PlayNote(double frequency)
{
	mAudioComponent->SetVolumeMultiplier(mSoundVolume);
	mAudioComponent->SetPitchMultiplier(frequency / mSoundFrequency);
	mAudioComponent->Play(0.00f);
}

/*
 * Stops playing the sound for this audio source.
 */
void AAudioSource::Stop(double fadeOutDuration)
{
	mAudioComponent->FadeOut(fadeOutDuration, 0.0f);
}

