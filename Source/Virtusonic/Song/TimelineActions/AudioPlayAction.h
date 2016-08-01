// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Audio/AudioSource.h"

#include "AudioPlayAction.generated.h"

/**
 * The song timeline action for playing a sound which is modulated by the given frequency.
 */
UCLASS()
class VIRTUSONIC_API UAudioPlayAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	// Initializes the action by specifying the audio source and the modulation frequency
	void Init(AAudioSource* audioSource, double frequency);

	virtual void Execute() override;
	
private:
	UPROPERTY()
	AAudioSource* _audioSource;

	double _frequency;
};
