// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "FretFingerController.generated.h"

UCLASS()
class VIRTUSONIC_API AFretFingerController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFretFingerController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
