// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "PickController.generated.h"

UCLASS()
class VIRTUSONIC_API APickController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
