// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Instrument/StringInstrument.h"
#include "Components/StaticMeshComponent.h"

#include "BassInstrument.generated.h"

/**
 *  The string instrument implementation of the 5-string bass guitar.
 */
UCLASS()
class VIRTUSONIC_API ABassInstrument : public AStringInstrument
{
	GENERATED_BODY()
	
public:
	// Called when the game starts or when spawned, used to provide references to the controller components
	virtual void BeginPlay() override;

	// Virtual function overrides
	virtual FString Name() override;
	virtual TArray<FStringPosition> GetPossibleStringPositions(int8 notePitch) override;
	virtual TArray<float> GetFretPositions() override;
	virtual FString GetPickAnimationPath() override;
	virtual FString GetFretFingerAnimationPath() override;
	virtual FString GetStringAnimationPath() override;
	virtual FString GetStringRoots() override;
	virtual int8 GetStringCount() override;
	virtual int8 GetFretCount() override;
};
