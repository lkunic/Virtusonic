// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "VirtusonicGameState.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API AVirtusonicGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSongFinished = false;
};
