// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "PickAnimator.generated.h"

UENUM()
enum class EPickAnimations : uint8
{
	RestToPickReadyX,
	PickReadyXToPickReadyY,
	PickReadyXToRest,
	PickX,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UPickAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickAnimator();

	static void LoadPickAnimations(FString assetPath);

	UAnimSequence* GetAnimationSequence(EPickAnimations anim, TCHAR X = '0', TCHAR Y = '0');

private:
	FString GetAnimationName(EPickAnimations anim, TCHAR X, TCHAR Y);

	static TMap<FString, UAnimSequence*> _pickAnimations;
	static UEnum* _pickAnimationNameEnum;
};
