// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringAnimator.h"


// Sets default values for this component's properties
UStringAnimator::UStringAnimator()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
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

// Static member declarations
TMap<FString, UAnimSequence*> UStringAnimator::mStringAnimations;
UEnum *UStringAnimator::mStringAnimationNameEnum;