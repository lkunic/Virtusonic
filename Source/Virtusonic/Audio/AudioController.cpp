// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioController.h"


// Sets default values for this component's properties
UAudioController::UAudioController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/*
* Adds an audio source to this component.
*/
void UAudioController::AddAudioSource(AAudioSource *audioSource)
{
	mAudioSources.Add(audioSource);
	mAlternateIndex.Add(0);
}

/*
 * Returns the number of audio sources attached to this component.
 */
int32 UAudioController::GetAudioSourceCount()
{
	return mAudioSources.Num();
}

/*
 * Returns the audio source at the given index.
 */
AAudioSource* UAudioController::GetAudioSource(int32 index)
{
	mAlternateIndex[index] = (mAlternateIndex[index] + 1) % 4;
	return mAudioSources[index + mAlternateIndex[index] * mAudioSources.Num() / 4];
}
