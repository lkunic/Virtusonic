// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/Actor.h"
#include "StringController.generated.h"

UCLASS()
class VIRTUSONIC_API AStringController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStringController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
