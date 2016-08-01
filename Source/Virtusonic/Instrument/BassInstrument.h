// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Instrument/StringInstrument.h"

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
	virtual TArray<StringPosition*> GetPossibleStringPositions(int32 notePitch) override;
	virtual FString GetPickAnimationPath() override;
	virtual FString GetStringRoots() override;
};
