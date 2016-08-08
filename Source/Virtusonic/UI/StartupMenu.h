// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Blueprint/UserWidget.h"
#include "StartupMenu.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API UStartupMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Song")
	TArray<FString> GetSongs();
};
