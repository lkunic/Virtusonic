// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringInstrument.h"

void AStringInstrument::SetControllerComponents(
	UAudioController* audioController, UFretFingerController* fretFingerController,
	UPickController* pickController, UStringController* stringController)
{
	_audioController = audioController;
	_fretFingerController = fretFingerController;
	_pickController = pickController;
	_stringController = stringController;
}


TArray<UBaseTimelineAction*> AStringInstrument::GenerateActions(USongNote* note)
{
	TArray<UBaseTimelineAction*> actions;

	GenerateAudioActions(&actions, note);
	GenerateFretFingerActions(&actions, note);
	GeneratePickActions(&actions, note);
	GenerateStringActions(&actions, note);
	
	return actions;
}

void AStringInstrument::GenerateAudioActions(TArray<UBaseTimelineAction*>* actions, USongNote* note)
{
	UAudioPlayAction* action = NewObject<UAudioPlayAction>();
	action->Init(_audioController->GetAudioSource(0), note->GetFrequency());
	action->Tick = note->GetStartTick();

	actions->Add(action);
}

void AStringInstrument::GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note)
{

}

void AStringInstrument::GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note)
{

}

void AStringInstrument::GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note)
{

}


