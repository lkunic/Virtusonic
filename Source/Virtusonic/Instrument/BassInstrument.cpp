// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "BassInstrument.h"

// Called when the game starts or when spawned
void ABassInstrument::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(VirtusonicLog, Log, TEXT("Added Bass instrument"));

	// Provide the references to the parent StringInstrument class which contains the implementation for playing string instruments
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

/*
 * Returns a list of possible string/fret positions for the given note.
 */
TArray<StringPosition*> ABassInstrument::GetPossibleStringPositions(int32 notePitch)
{
	// TODO for now it is just randomized, real algorithm needs to be implemented
	StringPosition position;
	//position.fret = FMath::RandHelper(24);
	//position.string = FMath::RandHelper(5);

	TArray<StringPosition*> result;
	//result.Add(&position);

	int32 fretboardRoots[] = { 23, 28, 33, 38, 43 };
	int32 fretCount = 24;

	for (int32 i = 0; i < 5; i++)
	{
		if (notePitch >= fretboardRoots[i] && notePitch <= fretboardRoots[i] + fretCount)
		{
			position.fret = notePitch - fretboardRoots[i];
			position.string = i;
			result.Add(&position);
		}
	}

	if (result.Num() == 0)
	{
		position.fret = FMath::RandHelper(24);
		position.string = FMath::RandHelper(5);
		result.Add(&position);
	}
	
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
