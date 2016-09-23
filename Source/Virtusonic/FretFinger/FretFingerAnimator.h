// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "FretFingerAnimator.generated.h"

#define DEFAULT_FRET_MOVE_DURATION 1.0f
#define DEFAULT_PIN_PRESS_DURATION 0.4f
#define DEFAULT_PIN_RELEASE_DURATION 0.8f

UENUM()
enum class EFretFingerAnimations : uint8
{
	FretFingerPrepare,
	FretFingerRest
};

USTRUCT()
struct FFretFingerState
{
	GENERATED_BODY()

	UPROPERTY()
	int8 Fret;

	UPROPERTY()
	float FretboardPosition;

	UPROPERTY()
	float PinSpacing;

	UPROPERTY()
	float FretboardPositionTime;
	
	UPROPERTY()
	TArray<float> PinPressValues;

	UPROPERTY()
	TArray<float> PinPressTimes;

	UPROPERTY()
	TArray<int32> PinNoteStartTick;

	FFretFingerState(int8 pinCount = 0)
	{
		Fret = -1;
		FretboardPosition = 0.0f;
		PinSpacing = 0.0f;
		FretboardPositionTime = 0.0f;
		PinPressValues.Init(0.0f, pinCount);
		PinPressTimes.Init(0.0f, pinCount);
		PinNoteStartTick.Init(0, pinCount);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUSONIC_API UFretFingerAnimator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFretFingerAnimator();

	static void LoadFretFingerAnimations(FString assetPath);

	void Init(const int8 stringCount, USkeletalMeshComponent *skeletalMesh, const TArray<float> &fretPositions, const FString &stringRoots);
	void Update(float deltaSeconds);

	void MoveToFret(int8 fret, float moveDuration = DEFAULT_FRET_MOVE_DURATION);
	void PressString(int8 string, int32 noteStartTick, float pressDuration = DEFAULT_PIN_PRESS_DURATION);
	void ReleaseString(int8 string, int32 noteStartTick, float releaseDuration = DEFAULT_PIN_RELEASE_DURATION);
	void ReturnToRest();

	UAnimSequence* GetAnimationSequence(EFretFingerAnimations anim);

private:
	float GetTargetPositionForFret(int8 fret);
	float GetPinSpacingForFret(int8 fret);
	float GetTimeLerp(float elapsedTime, float duration);

	static TMap<FString, UAnimSequence*> mFretFingerAnimations;
	static UEnum *mFretFingerAnimationNameEnum;

	int8 mStringCount;
	USkeletalMeshComponent *mSkeletalMesh;
	TArray<float> mFretPositions;
	FString mStringRoots;

	FVector mStartLocation;
	FFretFingerState mStartState;
	FFretFingerState mCurrentState;
	FFretFingerState mTargetState;
};
