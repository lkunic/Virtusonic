// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Audio/AudioSource.h"

#include "AudioStopAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UAudioStopAction : public UBaseTimelineAction
{
	GENERATED_BODY()
	
public:
	void Init(AAudioSource* audioSource);

	virtual void Execute() override;

private:
	AAudioSource* _audioSource;
};
