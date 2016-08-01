// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioSource.h"


// Sets default values
AAudioSource::AAudioSource()
{
	PrimaryActorTick.bCanEverTick = false;

	// Add an audio component to the actor
	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSource"));
	RootComponent = _audioComponent;
}

/*
 * Plays the stored sound at the given frequency.
 */
void AAudioSource::PlayNote(double frequency)
{
	_audioComponent->PitchMultiplier = frequency / _soundFrequency;
	_audioComponent->Play(0.30f);
}

/*
 * Stops playing the sound for this audio source.
 */
void AAudioSource::Stop()
{
	_audioComponent->Stop();
}

/*
 * Set the sound and the sound base frequency.
 */
void AAudioSource::SetSound(USoundCue* sound, float soundFrequency)
{
	_audioComponent->Sound = sound;
	_soundFrequency = soundFrequency;
}

