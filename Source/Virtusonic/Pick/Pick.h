// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "Pick.generated.h"

UCLASS()
class VIRTUSONIC_API APick : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APick();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
