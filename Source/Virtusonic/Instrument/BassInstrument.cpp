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
TArray<FStringPosition> ABassInstrument::GetPossibleStringPositions(int8 notePitch)
{
	TArray<FStringPosition> result;

	int32 fretboardRoots[] = { 47, 52, 57, 62, 67 };
	int32 fretCount = GetFretCount();

	for (int32 i = 0; i < 5; i++)
	{
		if (notePitch >= fretboardRoots[i] && notePitch <= fretboardRoots[i] + fretCount)
		{
			FStringPosition position;
			position.Fret = notePitch - fretboardRoots[i];
			position.String = i;
			result.Add(position);
		}
	}

	if (result.Num() == 0)
	{
		FStringPosition position;
		position.Fret = FMath::RandHelper(24);
		position.String = FMath::RandHelper(5);
		result.Add(position);
	}
	
	return result;
}

/*
 * Returns a list of fret positions as absolute offsets from the root position of a fret finger.
 */
TArray<float> ABassInstrument::GetFretPositions()
{
	TArray<float> fretPositions;
	fretPositions.Init(0.0f, GetFretCount() + 1);

	TArray<UActorComponent*> frets = GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("Frets"));

	for (UActorComponent* fret : frets)
	{
		int8 fretNumber = FCString::Atoi(*fret->GetName().Right(2));
		UStaticMeshComponent* mesh = (UStaticMeshComponent*)fret;
		FPositionVertexBuffer* buffer = &mesh->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;

		if (buffer)
		{
			float sum = 0;
			int32 numVertices = buffer->GetNumVertices();
			for (int32 iVertex = 0; iVertex < numVertices; iVertex++)
			{
				sum += buffer->VertexPosition(iVertex).Y;
			}
			fretPositions[fretNumber] = sum / numVertices;
		}
	}

	for (int i = 0; i < fretPositions.Num(); i++)
	{
		fretPositions[i] -= fretPositions.Last();
	}

	return fretPositions;
}

/*
 * Path to the pick animations.
 */
FString ABassInstrument::GetPickAnimationPath()
{
	return "/Game/Models/Bass/Pick";
}

/*
* Path to the fret finger animations.
*/
FString ABassInstrument::GetFretFingerAnimationPath()
{
	return "/Game/Models/Bass/FretFinger";
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

int8 ABassInstrument::GetFretCount()
{
	return 24;
}

int8 ABassInstrument::GetStringCount()
{
	return GetStringRoots().Len();
}