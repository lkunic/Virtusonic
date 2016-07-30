// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioController.h"


// Sets default values for this component's properties
UAudioController::UAudioController()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

/*
 * Returns the number of audio sources attached to this component.
 */
int32 UAudioController::GetAudioSourceCount()
{
	return _audioSources.Num();
}

/*
 * Returns the audio source at the given index.
 */
AAudioSource* UAudioController::GetAudioSource(int32 index)
{
	return _audioSources[index];
}

/*
 * Adds an audio source to this component.
 */
void UAudioController::AddAudioSource(AAudioSource* audioSource)
{
	_audioSources.Add(audioSource);
}


