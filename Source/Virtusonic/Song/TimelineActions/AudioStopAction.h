// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Audio/AudioSource.h"

#include "AudioStopAction.generated.h"

/**
 * The song timeline action for stopping the sound which is currently playing on the audio source.
 */
UCLASS()
class VIRTUSONIC_API UAudioStopAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	// Initializes the action by specifying the audio source
	void Init(AAudioSource *audioSource, float fadeOutDuration = 0.0f);

	virtual void Execute() override;

private:
	UPROPERTY()
	AAudioSource *mAudioSource;

	float mFadeOutDuration;
};
