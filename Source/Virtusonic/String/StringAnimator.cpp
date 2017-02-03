// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringAnimator.h"


// Sets default values for this component's properties
UStringAnimator::UStringAnimator()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
}

/*
 * Loads all animation sequences at the given path.
 */
void UStringAnimator::LoadStringAnimations(FString assetPath)
{
	// Load the assets at the given path
	auto objLib = UObjectLibrary::CreateLibrary(UAnimSequence::StaticClass(), false, true);
	TArray<FAssetData> assets;
	objLib->LoadAssetDataFromPath(*assetPath);
	objLib->GetAssetDataList(assets);

	// Process the assets as animation sequences and add them to the animation list
	FAssetData asset;
	FString assetName;
	UAnimSequence *anim;

	for (int32 i = 0; i < assets.Num(); i++)
	{
		asset = assets[i];
		assetName = asset.AssetName.ToString();
		anim = (UAnimSequence*)asset.GetAsset();
		mStringAnimations.Add(assetName, anim);
	}

	// Load the helper object used for converting enum values to strings
	mStringAnimationNameEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStringAnimations"), true);
}

void UStringAnimator::Init(USkeletalMeshComponent *skeletalMesh, const TArray<float> &fretPositions)
{
	mSkeletalMesh = skeletalMesh;
	mFretPositions = fretPositions;

	mStartState = FStringState(fretPositions.Num());
	mCurrentState = FStringState(fretPositions.Num());
	mTargetState = FStringState(fretPositions.Num());
}

void UStringAnimator::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (mCurrentState.PressValue != mTargetState.PressValue)
	{
		//UE_LOG(VirtusonicLog, Log, TEXT("CT: %.3f, TT: %.3f, CV: %.3f, TV: %.3f"), mCurrentState.PressTime, mTargetState.PressTime, mCurrentState.PressValue, mTargetState.PressValue);
		mCurrentState.PressTime += DeltaTime;
		float stringPressTime = GetTimeLerp(mCurrentState.PressTime, mTargetState.PressTime);

		mCurrentState.PressValue = mStartState.PressValue + stringPressTime * (mTargetState.PressValue - mStartState.PressValue);
	}
	
	//UE_LOG(VirtusonicLog, Log, TEXT("Setting fret%d to %.3f"), mCurrentState.Fret, mCurrentState.PressValue);
	if (mCurrentState.Fret != -1)
	{
		mSkeletalMesh->SetMorphTarget(*GetFretMorphTargetName(mCurrentState.Fret), mCurrentState.PressValue, false);
	}
	if (mCurrentState.PreviousFret != -1)
	{
		mSkeletalMesh->SetMorphTarget(*GetFretMorphTargetName(mCurrentState.PreviousFret), 1.0f - mCurrentState.PressValue, false);

		if (FMath::IsNearlyEqual(mCurrentState.PressValue, 1.0f))
		{
			mCurrentState.PreviousFret = -1;
		}
	}

	mSkeletalMesh->SetMorphTarget("Pressed", mCurrentState.PressValue, false);
}

void UStringAnimator::PressString(int8 fret, int32 noteStartTick, float pressDuration)
{
	//UE_LOG(VirtusonicLog, Log, TEXT("Pressing: %d"), fret);

	if (FMath::IsNearlyEqual(mCurrentState.PressValue, 0.0f))
	{
		mStartState.PressValue = 0.0f;
	}
	else if (mCurrentState.Fret != fret)
	{
		mStartState.PressValue = 1.0f - mCurrentState.PressValue;
		mCurrentState.PressValue = mStartState.PressValue;
		mCurrentState.PreviousFret = mCurrentState.Fret;
	}

	mCurrentState.PressTime = 0.0f;
	mCurrentState.Fret = fret;
	mCurrentState.NoteStartTick = noteStartTick;

	mTargetState.PressValue = 1.0f;
	mTargetState.PressTime = pressDuration;
}

void UStringAnimator::ReleaseString(int8 fret, int32 noteStartTick, float releaseDuration)
{
	//UE_LOG(VirtusonicLog, Log, TEXT("Releasing: %d"), fret);
	if (mCurrentState.Fret == fret && mCurrentState.NoteStartTick == noteStartTick)
	{
		mSkeletalMesh->Stop();

		//UE_LOG(VirtusonicLog, Log, TEXT("Executing release"));
		mStartState.PressValue = mCurrentState.PressValue;
		mCurrentState.PressTime = 0.0f;

		mTargetState.PressValue = 0.0f;
		mTargetState.PressTime = releaseDuration;
	}
}

void UStringAnimator::PlayStringVibration(UAnimSequence *vibrationAnimationSequence)
{
	if (mSkeletalMesh->IsPlaying())
	{
		mSkeletalMesh->Stop();
	}

	mSkeletalMesh->PlayAnimation(vibrationAnimationSequence, false);
}

/*
 * Returns the animation sequence matching the given template.
 */
UAnimSequence* UStringAnimator::GetAnimationSequence(EStringAnimations anim, TCHAR X)
{
	FString animName = GetAnimationName(anim, X);
	return mStringAnimations[animName];
}

/*
 * Helper function for assembling the animation name using the given parameters
 */
FString UStringAnimator::GetAnimationName(EStringAnimations anim, TCHAR X)
{
	FString x = "", y = "";
	FString animName = mStringAnimationNameEnum->GetEnumName((int32)anim);
	animName = animName.Replace(TEXT("X"), *x.AppendChar(X), ESearchCase::CaseSensitive);

	return animName;
}

float UStringAnimator::GetTimeLerp(float elapsedTime, float duration)
{
	float t = FMath::Min(1.0f, elapsedTime / duration);
	float tt = t * t;
	return 3 * tt - 2 * tt * t;
}

FString UStringAnimator::GetFretMorphTargetName(int8 fret)
{
	FString morphTargetName = "Fret";
	if (fret < 10)
	{
		morphTargetName.AppendChar('0');
	}

	morphTargetName.AppendInt((int32)fret);

	return morphTargetName;
}


// Static member declarations
TMap<FString, UAnimSequence*> UStringAnimator::mStringAnimations;
UEnum *UStringAnimator::mStringAnimationNameEnum;