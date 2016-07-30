// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "PickAnimator.h"


// Sets default values for this component's properties
UPickAnimator::UPickAnimator()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickAnimator::LoadPickAnimations(FString assetPath)
{
	auto objLib = UObjectLibrary::CreateLibrary(UAnimSequence::StaticClass(), false, true);
	TArray<FAssetData> assets;
	objLib->LoadAssetDataFromPath(*assetPath);
	objLib->GetAssetDataList(assets);

	FAssetData asset;
	FString assetName;
	UAnimSequence* anim;

	for (int32 i = 0; i < assets.Num(); i++)
	{
		asset = assets[i];
		assetName = asset.AssetName.ToString();
		anim = (UAnimSequence*)asset.GetAsset();
		//anim->RateScale = 1.5f;
		_pickAnimations.Add(assetName, anim);
	}

	_pickAnimationNameEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPickAnimations"), true);
}

UAnimSequence* UPickAnimator::GetAnimationSequence(EPickAnimations anim, TCHAR X, TCHAR Y)
{
	FString animName = GetAnimationName(anim, X, Y);
	return _pickAnimations[animName];
}

FString UPickAnimator::GetAnimationName(EPickAnimations anim, TCHAR X, TCHAR Y)
{
	FString x = "", y = "";
	FString animName = _pickAnimationNameEnum->GetEnumName((int32)anim);
	animName = animName.Replace(TEXT("X"), *x.AppendChar(X), ESearchCase::CaseSensitive);
	animName = animName.Replace(TEXT("Y"), *y.AppendChar(Y), ESearchCase::CaseSensitive);
	
	return animName;
}

TMap<FString, UAnimSequence*> UPickAnimator::_pickAnimations;
UEnum* UPickAnimator::_pickAnimationNameEnum;
