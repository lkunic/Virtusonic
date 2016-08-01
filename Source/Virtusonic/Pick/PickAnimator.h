// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "PickAnimator.generated.h"

/*
 * Enum containing templates for animation names.
 */
UENUM()
enum class EPickAnimations : uint8
{
	RestToPickReadyX,
	PickReadyXToPickReadyY,
	PickReadyXToRest,
	PickX,
};

/**
 * An actor component for managing pick animations.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UPickAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickAnimator();

	// Loads all animation sequences at the given path
	static void LoadPickAnimations(FString assetPath);

	// Returns the animation sequence matching the given template
	UAnimSequence* GetAnimationSequence(EPickAnimations anim, TCHAR X = '0', TCHAR Y = '0');

private:
	// Helper function for assembling the animation name using the given parameters
	FString GetAnimationName(EPickAnimations anim, TCHAR X, TCHAR Y);

	static TMap<FString, UAnimSequence*> _pickAnimations;
	static UEnum* _pickAnimationNameEnum;
};
