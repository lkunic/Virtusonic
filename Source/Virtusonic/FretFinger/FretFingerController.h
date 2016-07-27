// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "FretFingerController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UFretFingerController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFretFingerController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
