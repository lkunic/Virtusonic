// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "FretFingerController.h"


// Sets default values for this component's properties
UFretFingerController::UFretFingerController()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

/*
* Blueprint function for adding a fret finger to this controller.
*/
void UFretFingerController::AddFretFinger(AFretFinger* fretFinger)
{
	mFretFingers.Add(fretFinger);
}

/*
* Returns the number of fret fingers.
*/
int32 UFretFingerController::GetFretFingerCount()
{
	return mFretFingers.Num();
}

/*
* Returns the fret finger with the given index.
*/
AFretFinger* UFretFingerController::GetFretFinger(int32 index)
{
	return mFretFingers[index];
}