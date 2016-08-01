// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioStopAction.h"

/*
 * Initializes the action with the given audio source.
 */
void UAudioStopAction::Init(AAudioSource* audioSource)
{
	_audioSource = audioSource;
}

/*
 * Executes the audio stopping action.
 */
void UAudioStopAction::Execute()
{
	_audioSource->Stop();
	//UE_LOG(VirtusonicLog, Log, TEXT("Stopping sound on %s"), *(_audioSource->GetName()));
}




