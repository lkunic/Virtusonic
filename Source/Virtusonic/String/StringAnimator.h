// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "StringAnimator.generated.h"

#define DEFAULT_STRING_PRESS_DURATION 0.2f
#define DEFAULT_STRING_RELEASE_DURATION 0.4f

/*
* Enum containing templates for animation names.
*/
UENUM()
enum class EStringAnimations : uint8
{
	StringVibrationDownX,
	StringVibrationUpX
};

USTRUCT()
struct FStringState
{
	GENERATED_BODY()

	UPROPERTY()
	int8 Fret;

	UPROPERTY()
	int32 NoteStartTick;

	UPROPERTY()
	float PressValue;

	UPROPERTY()
	float PressTime;

	FStringState(int8 fretCount = 0)
	{
		Fret = -1;
		PressValue = 0.0;
		PressTime = 0.0;
	}
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

	void Init(USkeletalMeshComponent *skeletalMesh, const TArray<float> &fretPositions);
	void Update(float deltaSeconds);

	void PressString(int8 fret, int32 noteStartTick, float pressDuration = DEFAULT_STRING_PRESS_DURATION);
	void ReleaseString(int32 noteStartTick, float releaseDuration = DEFAULT_STRING_RELEASE_DURATION);

	// Returns the animation sequence matching the given template
	UAnimSequence* GetAnimationSequence(EStringAnimations anim, TCHAR X = '0');

private:
	// Helper function for assembling the animation name using the given parameters
	FString GetAnimationName(EStringAnimations anim, TCHAR X);
	float GetTimeLerp(float elapsedTime, float duration);
	FString GetFretMorphTargetName(int8 fret);

	static TMap<FString, UAnimSequence*> mStringAnimations;
	static UEnum *mStringAnimationNameEnum;

	USkeletalMeshComponent* mSkeletalMesh;
	TArray<float> mFretPositions;

	FStringState mStartState;
	FStringState mCurrentState;
	FStringState mTargetState;
};
