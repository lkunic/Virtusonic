// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "SongNote.generated.h"

#define NOTE_FREQUENCY_FACTOR 1.059463094359

/**
 * Container class for a single song note.
 */
UCLASS()
class VIRTUSONIC_API USongNote : public UObject
{
	GENERATED_BODY()

public:
	void Init(int32 startTick, int32 endTick, int32 pitch, int32 velocity);

	int32 GetStartTick();
	int32 GetEndTick();

	int32 GetPitch();
	int32 GetVelocity();
	double GetFrequency();

	void SetEndTick(int32 endTick);

private:
	int32 _startTick;
	int32 _endTick;
	int32 _pitch;
	int32 _velocity;
};
