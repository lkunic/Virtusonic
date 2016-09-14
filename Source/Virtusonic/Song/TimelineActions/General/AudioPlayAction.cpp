// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioPlayAction.h"

/*
 * Initializes the action with the given audio source and frequency.
 */
void UAudioPlayAction::Init(AAudioSource *audioSource, double frequency)
{
	mAudioSource = audioSource;
	mFrequency = frequency;
}

/*
 * Executes the action which plays the note.
 */
void UAudioPlayAction::Execute()
{
	mAudioSource->PlayNote(mFrequency);
	//UE_LOG(VirtusonicLog, Log, TEXT("Playing sound on %s - %.2f Hz"), *(_audioSource->GetName()), _frequency);
}


