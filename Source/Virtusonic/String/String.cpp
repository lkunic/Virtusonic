// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "String.h"


// Sets default values
AString::AString()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add a string animator component to the actor
	mAnimator = CreateDefaultSubobject<UStringAnimator>(TEXT("StringAnimator"));
}

void AString::Init(const TArray<float> &fretPositions)
{
	mAnimator->Init(GetSkeletalMeshComponent(), fretPositions);
}

void AString::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	//mAnimator->Update(deltaSeconds);
}

UStringAnimator* AString::GetAnimator()
{
	return mAnimator;
}