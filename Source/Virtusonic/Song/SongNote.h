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

	// Getters and setters
	int32 GetStartTick();
	int32 GetEndTick();
	void SetEndTick(int32 endTick);
	int32 GetPitch();
	int32 GetVelocity();
	double GetFrequency();

	// Needs to be a public member because it is used in the sorting predicate
	int32 StartTick;

private:
	int32 _endTick;
	int32 _pitch;
	int32 _velocity;
};
