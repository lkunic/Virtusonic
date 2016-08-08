// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Engine/GameInstance.h"
#include "VirtusonicGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UVirtusonicGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SongName;
};
