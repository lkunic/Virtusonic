// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFinger.h"

// Sets default values
AFretFinger::AFretFinger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add a fret finger animator component to the actor
	mAnimator = CreateDefaultSubobject<UFretFingerAnimator>(TEXT("FretFingerAnimator"));
}

void AFretFinger::Init(const int8 stringCount, const TArray<float> fretPositions, const FString &stringRoots)
{
	mAnimator->Init(stringCount, GetSkeletalMeshComponent(), fretPositions, stringRoots);

	TargetFret = -1;
	bIsResting = true;
	LastNoteTick = 0;
}

void AFretFinger::Tick(float deltaSeconds)
{
	mAnimator->Update(deltaSeconds);
}

UFretFingerAnimator* AFretFinger::GetAnimator()
{
	return mAnimator;
}



