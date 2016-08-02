// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringController.h"


// Sets default values for this component's properties
UStringController::UStringController()
{
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
}

/*
* Returns the number of strings.
*/
int32 UStringController::GetStringCount()
{
	return _strings.Num();
}

/*
* Returns the string with the given index.
*/
AString* UStringController::GetString(int32 index)
{
	return _strings[index];
}

/*
* Blueprint function for adding a string to this controller.
*/
void UStringController::AddString(AString* string)
{
	_strings.Add(string);
}
