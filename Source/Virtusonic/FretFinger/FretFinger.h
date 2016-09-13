// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "FretFingerAnimator.h"
#include "Runtime/Engine/Public/StaticMeshResources.h "

#include "FretFinger.generated.h"

UCLASS()
class VIRTUSONIC_API AFretFinger : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	AFretFinger();

	void Init(int8 stringCount, TArray<float> fretPositions);

	// The actor tick function that controls all interpolation animations
	virtual void Tick(float deltaSeconds) override;

	UFretFingerAnimator* GetAnimator();

private:

	UFretFingerAnimator *mAnimator;
};
