// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Object.h"
#include "Song/SongNote.h"

#include "StringInstrumentFingeringGraph.generated.h"

#define FINGER_PRESS_ANIM_DURATION 5

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
		for (int i = 0; i < IsPinPressing.Num(); i++)
		{
			if (IsPinPressing[i]) return true;
		}

		return false;
	}

	bool IsTransitioning() const
	{
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
	int16 ParentIndex;

	UPROPERTY()
	int32 ParentScore;

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
	void Init(const int8 fingerCount, const int8 stringCount);
	void AddNote(USongNote *note, const TArray<FStringPosition> &notePositions);
	void CalculateFingering();

	UPROPERTY()
	TArray<UGraphNode*> OptimalFingering;
	
private:
	void UpdateFingerboardStates(UGraphNode *node, const USongNote* note);
	void CalculateTransitionScores(UGraphNode *parentNode);
	bool IsTransitionPossible(const UGraphNode *node, const FFingerboardState &fingerboardState) const;
	FFingerboardState CreateTransition(const UGraphNode *node, const FFingerboardState &fingerboardState);
	int32 GetTransitionScore(const UGraphNode *node, const FFingerboardState &oldState, const FFingerboardState &newState);
	void BuildOptimalFingering(UGraphNode *node);

	int8 mFingerCount;
	int8 mStringCount;

	UPROPERTY()
	UGraphNode *mRoot;

	UPROPERTY()
	TArray<UGraphNode*> mLastLayer;
};
