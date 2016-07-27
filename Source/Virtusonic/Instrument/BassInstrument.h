// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Instrument/StringInstrument.h"

#include "BassInstrument.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API ABassInstrument : public AStringInstrument
{
	GENERATED_BODY()
	
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FString Name() override;
};
