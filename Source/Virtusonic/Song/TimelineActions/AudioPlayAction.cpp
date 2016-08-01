// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioPlayAction.h"

/*
 * Initializes the action with the given audio source and frequency.
 */
void UAudioPlayAction::Init(AAudioSource* audioSource, double frequency)
{
	_audioSource = audioSource;
	_frequency = frequency;
}

/*
 * Executes the action which plays the note.
 */
void UAudioPlayAction::Execute()
{
	_audioSource->PlayNote(_frequency);
	//UE_LOG(VirtusonicLog, Log, TEXT("Playing sound on %s - %.2f Hz"), *(_audioSource->GetName()), _frequency);
}


