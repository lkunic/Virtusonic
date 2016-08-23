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
	// Sets default values for this actor's properties
	AString();

	UStringAnimator* GetAnimator();

private:
	UStringAnimator *mAnimator;
};
