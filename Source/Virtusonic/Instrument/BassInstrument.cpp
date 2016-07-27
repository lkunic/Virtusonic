// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "BassInstrument.h"

// Called when the game starts or when spawned
void ABassInstrument::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(VirtusonicLog, Log, TEXT("Added Bass instrument"));

	UAudioController* audioController = (UAudioController*)GetComponentByClass(UAudioController::StaticClass());
	UFretFingerController* fretFingerController = (UFretFingerController*)GetComponentByClass(UFretFingerController::StaticClass());
	UPickController* pickController = (UPickController*)GetComponentByClass(UPickController::StaticClass());
	UStringController* stringController = (UStringController*)GetComponentByClass(UStringController::StaticClass());

	SetControllerComponents(audioController, fretFingerController, pickController, stringController);
}

FString ABassInstrument::Name()
{
	return "Bass";
}
