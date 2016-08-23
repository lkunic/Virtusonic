// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioStopAction.h"

/*
 * Initializes the action with the given audio source.
 */
void UAudioStopAction::Init(AAudioSource *audioSource, float fadeOutDuration)
{
	mAudioSource = audioSource;
	mFadeOutDuration = fadeOutDuration;
}

/*
 * Executes the audio stopping action.
 */
void UAudioStopAction::Execute()
{
	mAudioSource->Stop(mFadeOutDuration);
	//UE_LOG(VirtusonicLog, Log, TEXT("Stopping sound on %s"), *(_audioSource->GetName()));
}




