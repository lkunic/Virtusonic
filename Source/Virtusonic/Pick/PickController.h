// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Pick.h"

#include "PickController.generated.h"

/**
 * The string instrument component providing references to picks.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UPickController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickController();

	// The pick references are added through the actor blueprint for the instrument
	UFUNCTION(BlueprintCallable, Category = "Pick")
	void AddPick(APick *pick);

	// Public getters
	int32 GetPickCount();
	APick* GetPick(int32 index);


private:
	UPROPERTY(EditAnywhere, Category = "Pick")
	TArray<APick*> mPicks;
};
