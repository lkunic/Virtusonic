// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "StringAnimator.h"

#include "String.generated.h"

UCLASS()
class VIRTUSONIC_API AString : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	AString();

	void Init(const TArray<float> &fretPositions);

	// The actor tick function that controls all interpolation animations
	virtual void Tick(float deltaSeconds) override;

	UStringAnimator* GetAnimator();

private:
	UStringAnimator *mAnimator;
};
