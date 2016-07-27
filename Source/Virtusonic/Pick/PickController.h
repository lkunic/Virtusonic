// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Pick.h"

#include "PickController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UPickController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickController();

	// Called when the game starts
	virtual void BeginPlay() override;

	int32 GetPickCount();

	UFUNCTION(BlueprintCallable, Category = "Pick")
	void AddPick(APick* pick);

private:
	UPROPERTY(EditAnywhere, Category = "Pick")
	TArray<APick*> _picks;
};
