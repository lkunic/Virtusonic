// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerAnimator.h"

// Sets default values for this component's properties
UFretFingerAnimator::UFretFingerAnimator()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFretFingerAnimator::LoadFretFingerAnimations(FString assetPath)
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
		mFretFingerAnimations.Add(assetName, anim);
	}

	// Load the helper object used for converting enum values to strings
	mFretFingerAnimationNameEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EFretFingerAnimations"), true);
}

void UFretFingerAnimator::Init(const int8 stringCount, USkeletalMeshComponent *skeletalMesh, const TArray<float> &fretPositions, const FString &stringRoots)
{
	mStringCount = stringCount;
	mSkeletalMesh = skeletalMesh;
	mFretPositions = fretPositions;
	mStringRoots = stringRoots;

	mStartLocation = FVector(skeletalMesh->RelativeLocation);
	mStartState = FFretFingerState(stringCount);
	mCurrentState = FFretFingerState(stringCount);
	mTargetState = FFretFingerState(stringCount);

	mCurrentState.FretboardPosition = mStartLocation.Y;
}

/*
 * The update function that gets called on every tick and updates the finger mesh based on the target state.
 */
void UFretFingerAnimator::Update(float deltaSeconds)
{
	// Update the finger position along the fretboard if necessary
	// (if the target time is zero, the finger doesn't need to be moved)
	if (mCurrentState.Fret != mTargetState.Fret)
	{
		// If the finger hasn't yet reached the destination, update its position
		if (mCurrentState.FretboardPositionTime < mTargetState.FretboardPositionTime)
		{
			mCurrentState.FretboardPositionTime += deltaSeconds;
			float fretboardPositionTime = GetTimeLerp(mCurrentState.FretboardPositionTime, mTargetState.FretboardPositionTime);

			mCurrentState.FretboardPosition = mStartState.FretboardPosition +
				fretboardPositionTime * (mTargetState.FretboardPosition - mStartState.FretboardPosition);

			mCurrentState.PinSpacing = mStartState.PinSpacing +
				fretboardPositionTime * (mTargetState.PinSpacing - mStartState.PinSpacing);
		}
		else
		{
			// The finger has reached a point at or slightly after the target position, 
			// so move it to the exact position and disable further movement
			mCurrentState.FretboardPosition = mTargetState.FretboardPosition;
			mCurrentState.PinSpacing = mTargetState.PinSpacing;
			mCurrentState.Fret = mTargetState.Fret;
		}

		// Translate the mesh along the fretboard
		mSkeletalMesh->SetRelativeLocation(FVector(mStartLocation.X, mCurrentState.FretboardPosition, mStartLocation.Z));
		mSkeletalMesh->SetMorphTarget("StringSpacing", mCurrentState.PinSpacing);
	}

	for (int8 iPin = 0; iPin < mStringCount; iPin++)
	{
		if (mCurrentState.PinPressValues[iPin] != mTargetState.PinPressValues[iPin])
		{
			if (mCurrentState.PinPressTimes[iPin] < mTargetState.PinPressTimes[iPin])
			{
				mCurrentState.PinPressTimes[iPin] += deltaSeconds;
				float pinPressTime = GetTimeLerp(mCurrentState.PinPressTimes[iPin], mTargetState.PinPressTimes[iPin]);

				mCurrentState.PinPressValues[iPin] = mStartState.PinPressValues[iPin] +
					pinPressTime * (mTargetState.PinPressValues[iPin] - mStartState.PinPressValues[iPin]);
			}
			else
			{
				mCurrentState.PinPressValues[iPin] = mTargetState.PinPressValues[iPin];
			}
		}

		FString morphTargetName = "FingerPress";

		mSkeletalMesh->SetMorphTarget(*morphTargetName.AppendChar(mStringRoots[iPin]), mCurrentState.PinPressValues[iPin]);
	}
}

void UFretFingerAnimator::MoveToFret(int8 fret, float moveDuration)
{
	mStartState.FretboardPosition = mCurrentState.FretboardPosition;
	mStartState.PinSpacing = mCurrentState.PinSpacing;

	mCurrentState.FretboardPositionTime = 0.0f;
	mCurrentState.Fret = -1;

	mTargetState.Fret = fret;
	mTargetState.FretboardPosition = GetTargetPositionForFret(fret);
	mTargetState.PinSpacing = GetPinSpacingForFret(fret);
	mTargetState.FretboardPositionTime = moveDuration;
}

void UFretFingerAnimator::PressString(int8 string, int32 noteStartTick, float pressDuration)
{
	mStartState.PinPressValues[string] = mCurrentState.PinPressValues[string];

	mCurrentState.PinPressTimes[string] = 0.0f;
	mCurrentState.PinNoteStartTick[string] = noteStartTick;

	mTargetState.PinPressValues[string] = 0.6f;
	mTargetState.PinPressTimes[string] = pressDuration;
}

void UFretFingerAnimator::ReleaseString(int8 string, int32 noteStartTick, float releaseDuration)
{
	if (mCurrentState.PinNoteStartTick[string] == noteStartTick)
	{
		mStartState.PinPressValues[string] = mCurrentState.PinPressValues[string];

		mCurrentState.PinPressTimes[string] = 0.0f;

		mTargetState.PinPressValues[string] = 0.0f;
		mTargetState.PinPressTimes[string] = releaseDuration;
	}
}

void UFretFingerAnimator::ReturnToRest()
{
	mStartState.FretboardPosition = mCurrentState.FretboardPosition;
	mStartState.PinSpacing = mCurrentState.PinSpacing;

	mCurrentState.FretboardPositionTime = 0.0f;

	mTargetState.Fret = -1;
	mTargetState.FretboardPosition = mStartLocation.Y;
	mTargetState.PinSpacing = 0.0f;
	mTargetState.FretboardPositionTime = DEFAULT_FRET_MOVE_DURATION;
}

UAnimSequence* UFretFingerAnimator::GetAnimationSequence(EFretFingerAnimations anim)
{
	FString animName = mFretFingerAnimationNameEnum->GetEnumName((int32)anim);
	return mFretFingerAnimations[animName];
}

float UFretFingerAnimator::GetTargetPositionForFret(int8 fret)
{
	return mFretPositions[fret] - 0.2;
}

float UFretFingerAnimator::GetPinSpacingForFret(int8 fret)
{
	return 1.0f - mFretPositions[fret] / mFretPositions[1];
}

float UFretFingerAnimator::GetTimeLerp(float elapsedTime, float duration)
{
	float t = elapsedTime / duration;
	float tt = t * t;
	return 3 * tt - 2 * tt * t;
}

TMap<FString, UAnimSequence*> UFretFingerAnimator::mFretFingerAnimations;
UEnum *UFretFingerAnimator::mFretFingerAnimationNameEnum;
