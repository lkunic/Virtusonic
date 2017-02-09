// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "Song/SongNote.h"

#include "StringInstrumentFingeringGraph.generated.h"

#define FINGER_PRESS_ANIM_DURATION 5

// Scoring constants
#define EMPTY_STRING_COST 400
#define FRET_POSITION_FACTOR 6
#define FRET_DISTANCE_FACTOR 2
#define FINGER_SPAN_FACTOR 2

USTRUCT()
struct VIRTUSONIC_API FStringPosition
{
	GENERATED_BODY()

	UPROPERTY()
	int8 String;

	UPROPERTY()
	int8 Fret;
};

USTRUCT()
struct VIRTUSONIC_API FFingerState
{
	GENERATED_BODY()

	UPROPERTY()
	int8 Fret;

	UPROPERTY()
	TArray<bool> IsPinPressing;

	UPROPERTY()
	TArray<int32> PinPressEndTick;

	bool IsPressing() const
	{
		// Check if a pin is currently pressing a string
		for (int i = 0; i < IsPinPressing.Num(); i++)
		{
			if (IsPinPressing[i]) return true;
		}

		return false;
	}

	bool IsTransitioning() const
	{
		// Check if a pin was recently pressing a string and is now in the transition phase
		// before it can be used again for pressing a string on a different fret
		for (int i = 0; i < PinPressEndTick.Num(); i++)
		{
			if (PinPressEndTick[i] != 0) return true;
		}

		return false;
	}
};

USTRUCT()
struct VIRTUSONIC_API FFingerboardState
{
	GENERATED_BODY()

	UPROPERTY()
	int8 ParentIndex;

	UPROPERTY()
	int32 ParentScore;

	UPROPERTY()
	int8 ParentFingerboardStateIndex;

	UPROPERTY()
	TArray<FFingerState> FingerStates;
};

UCLASS()
class VIRTUSONIC_API UGraphNode : public UObject
{
	GENERATED_BODY()

public:
	FStringPosition StringPosition;
	int8 FingerIndex;

	UPROPERTY()
	USongNote *Note;

	UPROPERTY()
	TArray<FFingerboardState> FingerboardStates;

	UPROPERTY()
	TArray<UGraphNode*> Parents;

	UPROPERTY()
	TArray<UGraphNode*> Children;
};

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UStringInstrumentFingeringGraph : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(const int8 fingerCount, const int8 stringCount, const int8 fretCount);
	void AddNote(USongNote *note, const TArray<FStringPosition> &notePositions);
	void CalculateFingering();

	UPROPERTY()
	TArray<UGraphNode*> OptimalFingering;
	
private:
	void UpdateFingerboardStates(UGraphNode *node, const int32 tick);
	void CalculateTransitionScores(UGraphNode *parentNode);

	bool CreateTransitionFingerboardState(const UGraphNode *node, const FFingerboardState &fingerboardState, FFingerboardState &outState);
	int8 GetHighestFretForFinger(const int8 fingerIndex, const FFingerboardState &fingerboardState);
	int8 GetLowestFretForFinger(const int8 fingerIndex, const FFingerboardState &fingerboardState);

	int32 GetTransitionScore(const UGraphNode *node, const FFingerboardState &oldState, const FFingerboardState &newState);
	void BuildOptimalFingering(UGraphNode *node, const FFingerboardState &state);
	bool CleanupFingerMovement(const int32 noteIndex, const int8 fingerIndex, const int8 lastPressedFret);

	int8 mFingerCount;
	int8 mStringCount;
	int8 mFretCount;

	UPROPERTY()
	UGraphNode *mRoot;

	UPROPERTY()
	TArray<UGraphNode*> mLastLayer;
};
