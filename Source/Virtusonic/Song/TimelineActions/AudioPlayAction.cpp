// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioPlayAction.h"

void UAudioPlayAction::Init(AAudioSource* audioSource, double frequency)
{
	_audioSource = audioSource;
	_frequency = frequency;
}

void UAudioPlayAction::Execute()
{
	_audioSource->PlayNote(_frequency);
}


