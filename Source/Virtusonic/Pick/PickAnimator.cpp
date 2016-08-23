// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickAnimator.h"

// Sets default values for this component's properties
UPickAnimator::UPickAnimator()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

/*
 * Loads all animation sequences at the given path.
 */
void UPickAnimator::LoadPickAnimations(FString assetPath)
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
		mPickAnimations.Add(assetName, anim);
	}

	// Load the helper object used for converting enum values to strings
	mPickAnimationNameEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPickAnimations"), true);
}

/*
 * Returns the animation sequence matching the given template.
 */
UAnimSequence* UPickAnimator::GetAnimationSequence(EPickAnimations anim, TCHAR X, TCHAR Y)
{
	FString animName = GetAnimationName(anim, X, Y);
	return mPickAnimations[animName];
}

 /*
  * Helper function for assembling the animation name using the given parameters
  */
FString UPickAnimator::GetAnimationName(EPickAnimations anim, TCHAR X, TCHAR Y)
{
	FString x = "", y = "";
	FString animName = mPickAnimationNameEnum->GetEnumName((int32)anim);
	animName = animName.Replace(TEXT("X"), *x.AppendChar(X), ESearchCase::CaseSensitive);
	animName = animName.Replace(TEXT("Y"), *y.AppendChar(Y), ESearchCase::CaseSensitive);
	
	return animName;
}

// Static member declarations
TMap<FString, UAnimSequence*> UPickAnimator::mPickAnimations;
UEnum *UPickAnimator::mPickAnimationNameEnum;
