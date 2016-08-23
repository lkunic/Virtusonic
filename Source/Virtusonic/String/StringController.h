// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "String.h"

#include "StringController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UStringController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStringController();

	// The string references are added through the actor blueprint for the instrument
	UFUNCTION(BlueprintCallable, Category = "String")
	void AddString(AString *string);

	// Public getters
	int32 GetStringCount();
	AString* GetString(int32 index);

private:
	UPROPERTY(EditAnywhere, Category = "String")
	TArray<AString*> mStrings;
};
