// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "String.h"


// Sets default values
AString::AString()
{
	PrimaryActorTick.bCanEverTick = false;

	// Add a string animator component to the actor
	_animator = CreateDefaultSubobject<UStringAnimator>(TEXT("StringAnimator"));
}

UStringAnimator* AString::GetAnimator()
{
	return _animator;
}