// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "BassInstrument.h"

// Called when the game starts or when spawned
void ABassInstrument::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(VirtusonicLog, Log, TEXT("Added Bass instrument"));

	// Provide the references to the parent StringInstrument class which contains the implementation for playing string instruments
	UAudioController *audioController = (UAudioController*)GetComponentByClass(UAudioController::StaticClass());
	UFretFingerController *fretFingerController = (UFretFingerController*)GetComponentByClass(UFretFingerController::StaticClass());
	UPickController *pickController = (UPickController*)GetComponentByClass(UPickController::StaticClass());
	UStringController *stringController = (UStringController*)GetComponentByClass(UStringController::StaticClass());

	SetControllerComponents(audioController, fretFingerController, pickController, stringController);
}

FString ABassInstrument::Name()
{
	return "Bass";
}

/*
 * Returns a list of possible string/fret positions for the given note.
 */
TArray<FStringPosition*> ABassInstrument::GetPossibleStringPositions(int32 notePitch)
{
	// TODO for now it is just randomized, real algorithm needs to be implemented
	FStringPosition position;
	position.fret = FMath::RandHelper(24);
	position.string = FMath::RandHelper(5);

	TArray<FStringPosition*> result;
	result.Add(&position);
	
	return result;
}

/*
 * Path to the pick animations.
 */
FString ABassInstrument::GetPickAnimationPath()
{
	return "/Game/Models/Bass/Pick";
}

/*
 * Path to the string animations.
 */
FString ABassInstrument::GetStringAnimationPath()
{
	return "/Game/Models/Bass/Strings";
}

/*
 * String roots for the bass guitar, starting from the top (lowest pitch) string.
 */
FString ABassInstrument::GetStringRoots()
{
	return "BEADG";
}
