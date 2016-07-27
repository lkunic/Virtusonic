// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "AudioSource.h"


// Sets default values
AAudioSource::AAudioSource()
{
	PrimaryActorTick.bCanEverTick = false;

	_audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSource"));
	RootComponent = _audioComponent;
}

// Called when the game starts or when spawned
void AAudioSource::BeginPlay()
{
	Super::BeginPlay();
}

void AAudioSource::PlayNote(double frequency)
{
	_audioComponent->PitchMultiplier = frequency / _soundFrequency;
	_audioComponent->Play(0.15f);

	UE_LOG(VirtusonicLog, Log, TEXT("Playing note: %.2f using sound %.2f"), frequency, _soundFrequency);
}

void AAudioSource::SetSound(USoundCue* sound, float soundFrequency)
{
	_audioComponent->Sound = sound;
	_soundFrequency = soundFrequency;
}

