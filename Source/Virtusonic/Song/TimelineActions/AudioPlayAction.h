// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Song/TimelineActions/BaseTimelineAction.h"
#include "Audio/AudioSource.h"

#include "AudioPlayAction.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UAudioPlayAction : public UBaseTimelineAction
{
	GENERATED_BODY()

public:
	void Init(AAudioSource* audioSource, double frequency);

	virtual void Execute() override;
	
private:	
	AAudioSource* _audioSource;
	double _frequency;
};
