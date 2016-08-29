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

	bool IsPressing()
	{
		for (int i = 0; i < IsPinPressing.Num(); i++)
		{
			if (IsPinPressing[i]) return true;
		}

		return false;
	}
};

UCLASS()
class VIRTUSONIC_API UNoteConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FFingerState> FingerStates;

	UPROPERTY()
	UNoteConfig *Parent;

	UPROPERTY()
	TArray<UNoteConfig*> Children;
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
	void AddNote(USongNote *note, const TArray<FStringPosition> notePositions);
	
private:
	UNoteConfig* CreateNoteConfig(const UNoteConfig *parentConfig, USongNote *note);
	void DeleteNoteConfig(UNoteConfig *config);

	bool PositionAvailable(UNoteConfig *currentConfig, const FStringPosition position);
	bool CanPlayNote(UNoteConfig *currentConfig, const FStringPosition position, int8 fingerIndex);

	int8 mFingerCount;

	UPROPERTY()
	UNoteConfig *mRoot;

	UPROPERTY()
	TArray<UNoteConfig*> mLastLayer;
};
