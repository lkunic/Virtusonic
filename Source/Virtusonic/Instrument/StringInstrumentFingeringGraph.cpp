// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringInstrumentFingeringGraph.h"

void UStringInstrumentFingeringGraph::Init(const int8 fingerCount, const int8 stringCount)
{
	mFingerCount = fingerCount;
	mRoot = NewObject<UNoteConfig>();
	for (int i = 0; i < fingerCount; i++)
	{
		FFingerState fingerState;
		fingerState.Fret = 0;
		for (int j = 0; j < stringCount; j++)
		{
			fingerState.IsPinPressing.Add(false);
			fingerState.PinPressEndTick.Add(0);
		}
		mRoot->FingerStates.Add(fingerState);
	}

	mRoot->Parent = nullptr;
	mRoot->Children.Empty();

	mLastLayer.Add(mRoot);
}

void UStringInstrumentFingeringGraph::AddNote(USongNote *note, const TArray<FStringPosition> notePositions)
{
	TArray<UNoteConfig*> currentLayer;

	for (int iParentConfig = 0; iParentConfig < mLastLayer.Num(); iParentConfig++)
	{
		UNoteConfig *baseCurrentConfig = CreateNoteConfig(mLastLayer[iParentConfig], note);
		for (FStringPosition position : notePositions)
		{
			if (!PositionAvailable(baseCurrentConfig, position)) continue;

			if (position.Fret == 0)
			{
				// Note is on an empty string, the finger states don't change
				UNoteConfig *newConfig = DuplicateObject<UNoteConfig>(baseCurrentConfig, baseCurrentConfig->GetOuter());
				newConfig->Parent = mLastLayer[iParentConfig];
				mLastLayer[iParentConfig]->Children.Add(newConfig);
				currentLayer.Add(newConfig);
			}
			else
			{
				for (int i = 0; i < mFingerCount; i++)
				{
					if (CanPlayNote(baseCurrentConfig, position, i))
					{
						UNoteConfig *newConfig = DuplicateObject<UNoteConfig>(baseCurrentConfig, baseCurrentConfig->GetOuter());
						newConfig->FingerStates[i].Fret = position.Fret;
						newConfig->FingerStates[i].IsPinPressing[position.String] = true;
						newConfig->FingerStates[i].PinPressEndTick[position.String] = note->GetEndTick();
						newConfig->Parent = mLastLayer[iParentConfig];
						mLastLayer[iParentConfig]->Children.Add(newConfig);
						currentLayer.Add(newConfig);
					}
				}
			}
		}
	}

	for (UNoteConfig *parentConfig : mLastLayer)
	{
		if (parentConfig->Children.Num() == 0)
		{
			DeleteNoteConfig(parentConfig);
		}
	}

	mLastLayer = currentLayer;
}

UNoteConfig* UStringInstrumentFingeringGraph::CreateNoteConfig(const UNoteConfig *parentConfig, USongNote *note)
{
	UNoteConfig *result = DuplicateObject<UNoteConfig>(parentConfig, parentConfig->GetOuter());
	for (int iFingerState = 0; iFingerState < result->FingerStates.Num(); iFingerState++)
	{
		for (int i = 0, n = result->FingerStates[iFingerState].IsPinPressing.Num(); i < n; i++)
		{
			if (result->FingerStates[iFingerState].IsPinPressing[i] && 
				result->FingerStates[iFingerState].PinPressEndTick[i] < note->StartTick - FINGER_PRESS_ANIM_DURATION)
			{
				result->FingerStates[iFingerState].IsPinPressing[i] = false;
			}
		}
	}

	return result;
}

void UStringInstrumentFingeringGraph::DeleteNoteConfig(UNoteConfig *config)
{
	if (config->Parent == nullptr) return;

	config->Parent->Children.Remove(config);

	if (config->Parent->Children.Num() == 0)
	{
		DeleteNoteConfig(config->Parent);
	}
}


bool UStringInstrumentFingeringGraph::PositionAvailable(UNoteConfig *currentConfig, const FStringPosition position)
{
	for (int iFingerState = 0; iFingerState < currentConfig->FingerStates.Num(); iFingerState++)
	{
		FFingerState fingerState = currentConfig->FingerStates[iFingerState];
		if (fingerState.IsPinPressing[position.String] && fingerState.Fret != position.Fret)
		{
			return false;
		}
	}

	return true;
}


bool UStringInstrumentFingeringGraph::CanPlayNote(UNoteConfig *currentConfig, const FStringPosition position, int8 fingerIndex)
{
	FFingerState fingerState = currentConfig->FingerStates[fingerIndex];
	
	// If the finger is already on the correct fret, return true
	if (fingerState.Fret == position.Fret)
	{
		return true;
	}

	// If the finger is not on the correct fret and is currently pressing, return false
	if (fingerState.Fret != position.Fret && currentConfig->FingerStates[fingerIndex].IsPressing())
	{
		return false;
	}

	for (int i = 0, n = currentConfig->FingerStates.Num(); i < n; i++)
	{
		// If a lower index finger is currently pressing between this finger and the target fret
		if (i < fingerIndex && currentConfig->FingerStates[i].Fret >= position.Fret && currentConfig->FingerStates[i].IsPressing())
		{
			return false;
		}

		// If a higher index finger is currently pressing between this finger and the target fret
		if (i > fingerIndex && currentConfig->FingerStates[i].Fret <= position.Fret && currentConfig->FingerStates[i].IsPressing())
		{
			return false;
		}
	}

	// No restrictions are violated
	return true;
}



