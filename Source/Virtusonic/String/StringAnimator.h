// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "StringAnimator.generated.h"

/*
* Enum containing templates for animation names.
*/
UENUM()
enum class EStringAnimations : uint8
{
	StringVibrationDownX,
	StringVibrationUpX
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UStringAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStringAnimator();

	// Loads all animation sequences at the given path
	static void LoadStringAnimations(FString assetPath);

	// Returns the animation sequence matching the given template
	UAnimSequence* GetAnimationSequence(EStringAnimations anim, TCHAR X = '0');

private:
	// Helper function for assembling the animation name using the given parameters
	FString GetAnimationName(EStringAnimations anim, TCHAR X);

	static TMap<FString, UAnimSequence*> _stringAnimations;
	static UEnum* _stringAnimationNameEnum;
};
