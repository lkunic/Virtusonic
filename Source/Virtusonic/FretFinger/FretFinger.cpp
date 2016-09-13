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

void AFretFinger::Init(int8 stringCount, TArray<float> fretPositions)
{
	mAnimator->Init(stringCount, GetSkeletalMeshComponent());
}

void AFretFinger::Tick(float deltaSeconds)
{
	mAnimator->Update(deltaSeconds);
}

UFretFingerAnimator* AFretFinger::GetAnimator()
{
	return mAnimator;
}
