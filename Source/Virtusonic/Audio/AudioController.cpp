// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioController.h"


// Sets default values for this component's properties
UAudioController::UAudioController()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAudioController::BeginPlay()
{
	Super::BeginPlay();
}

int32 UAudioController::GetAudioSourceCount()
{
	return _audioSources.Num();
}

AAudioSource* UAudioController::GetAudioSource(int32 index)
{
	return _audioSources[index];
}


void UAudioController::AddAudioSource(AAudioSource* audioSource)
{
	_audioSources.Add(audioSource);
}


