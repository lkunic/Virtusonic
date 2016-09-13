// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FretFingerAnimator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UFretFingerAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFretFingerAnimator();

	void Init(int8 stringCount, USkeletalMeshComponent *skeletalMesh);
	void Update(float deltaSeconds);

private:
	USkeletalMeshComponent *mSkeletalMesh;

	float GetTargetPositionForFret(int8 fret);

	int8 mStringCount;
	float mCurrentFretboardPosition;
	float mTargetFretboardPosition;

	FVector mStartLocation;

	TArray<float> mCurrentPinPressValues;
	TArray<float> mTargetPinPressValues;
};
