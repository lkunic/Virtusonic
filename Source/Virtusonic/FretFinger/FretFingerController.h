// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "FretFinger.h"

#include "FretFingerController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UFretFingerController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFretFingerController();

	// The fret finger references are added through the actor blueprint for the instrument
	UFUNCTION(BlueprintCallable, Category = "FretFinger")
	void AddFretFinger(AFretFinger *fretFinger);

	// Public getters
	int32 GetFretFingerCount();
	AFretFinger* GetFretFinger(int32 index);

private:
	UPROPERTY(EditAnywhere, Category = "FretFinger")
	TArray<AFretFinger*> mFretFingers;
};
