// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerAnimator.h"

// Sets default values for this component's properties
UFretFingerAnimator::UFretFingerAnimator()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UFretFingerAnimator::Init(int8 stringCount, USkeletalMeshComponent *skeletalMesh)
{
	mStringCount = stringCount;

	mSkeletalMesh = skeletalMesh;
	mStartLocation = skeletalMesh->RelativeLocation;

	mCurrentFretboardPosition = mStartLocation.Y;
	mTargetFretboardPosition = mStartLocation.Y;

	for (int8 i = 0; i < stringCount; i++)
	{
		mCurrentPinPressValues.Add(0.0f);
		mTargetPinPressValues.Add(0.0f);
	}
}

void UFretFingerAnimator::Update(float deltaSeconds)
{

	//mSkeletalMesh->SetRelativeLocation(mSkeletalMesh->RelativeLocation + FVector(0, 0.01, 0));
}

float UFretFingerAnimator::GetTargetPositionForFret(int8 fret)
{
	
	return 0.0f;
}
