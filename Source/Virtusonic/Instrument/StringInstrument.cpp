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

TArray<UBaseTimelineAction*> AStringInstrument::GenerateActions(TArray<USongNote*> notes)
{
	TArray<UBaseTimelineAction*> actions;
	StringPosition position;

	_songLength = notes.Last()->GetEndTick() + 4 * _ticksPerQuarter;

	InitPicks();

	for (int i = 0; i < notes.Num(); i++)
	{
		position = GetStringPositionForNote(notes[i]);

		GenerateAudioActions(&actions, notes[i], position);
		GenerateFretFingerActions(&actions, notes[i], position);
		GeneratePickActions(&actions, notes[i], position);
		GenerateStringActions(&actions, notes[i], position);
	}

	APick* pick;
	for (int i = 0; i < _pickController->GetPickCount(); i++)
	{
		pick = _pickController->GetPick(i);
		ReturnPickToRest(&actions, pick);
	}
	
	return actions;
}

StringPosition AStringInstrument::GetStringPositionForNote(USongNote* note)
{
	return *GetPossibleStringPositions(note->GetPitch())[0];
}

TArray<StringPosition*> AStringInstrument::GetPossibleStringPositions(int32 notePitch)
{
	return TArray<StringPosition*>();
}

FString AStringInstrument::GetPickAnimationPath()
{
	return "Game/Models";
}

FString AStringInstrument::GetStringRoots()
{
	return "EADGBE";
}

void AStringInstrument::InitPicks()
{
	APick* pick;
	for (int i = 0; i < _pickController->GetPickCount(); i++)
	{
		pick = _pickController->GetPick(i);
		pick->Init(_tempo, _ticksPerQuarter, _songLength, GetStringRoots());
	}

	UPickAnimator::LoadPickAnimations(GetPickAnimationPath());
}

void AStringInstrument::ReturnPickToRest(TArray<UBaseTimelineAction*>* actions, APick* pick)
{
	int32 lastPlayTick = pick->GetLastPlayTick();
	if (lastPlayTick != 0 && pick->TimelineStatus(lastPlayTick) != 'R')
	{
		UPickRestAction* restAction = NewObject<UPickRestAction>();
		restAction->Init(pick, pick->TimelineStatus(lastPlayTick));
		restAction->Tick = lastPlayTick;
		actions->Add(restAction);

		int32 restAnimTicks = pick->SequenceLengthInTicks(restAction->GetAnimationLength());
		pick->UpdateTimeline(lastPlayTick, lastPlayTick + restAnimTicks, 'P');
		pick->UpdateTimeline(lastPlayTick + restAnimTicks, _songLength, 'R');
	}
}

void AStringInstrument::GenerateAudioActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{
	UAudioPlayAction* playAction = NewObject<UAudioPlayAction>();
	playAction->Init(_audioController->GetAudioSource(stringPosition.string), note->GetFrequency());
	playAction->Tick = note->GetStartTick();
	actions->Add(playAction);

	UAudioStopAction* stopAction = NewObject<UAudioStopAction>();
	stopAction->Init(_audioController->GetAudioSource(stringPosition.string));
	stopAction->Tick = note->GetEndTick();
	actions->Add(stopAction);
}

void AStringInstrument::GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{

}

void AStringInstrument::GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{
	APick* pick;
	TCHAR stringRoot = GetStringRoots()[stringPosition.string];
	int32 noteTick = note->GetStartTick();
	int32 lastTick;

	APick* bestPick = _pickController->GetPick(0);
	int32 bestTick = _songLength;

	for (int i = 0; i < _pickController->GetPickCount(); i++)
	{
		pick = _pickController->GetPick(i);
		lastTick = pick->GetLastPlayTick();
		if (lastTick < bestTick)
		{
			bestPick = pick;
			bestTick = lastTick;
		}

		if (pick->CanPlayNote(noteTick, stringPosition.string))
		{
			if (pick->CanRestBeforePick(noteTick))
			{
				ReturnPickToRest(actions, pick);
			}

			if (pick->TimelineStatus(noteTick) != stringRoot)
			{
				UPickPrepareAction* prepareAction = NewObject<UPickPrepareAction>();
				prepareAction->Init(pick, pick->TimelineStatus(noteTick), stringRoot);

				int32 prepareAnimTicks = pick->SequenceLengthInTicks(prepareAction->GetAnimationLength());
				prepareAction->Tick = noteTick - prepareAnimTicks;
				actions->Add(prepareAction);

				pick->UpdateTimeline(noteTick - prepareAnimTicks, noteTick, 'P');
			}

			UPickPlayAction* playAction = NewObject<UPickPlayAction>();
			playAction->Init(pick, stringRoot);
			playAction->Tick = noteTick;
			actions->Add(playAction);

			int32 playAnimTicks = pick->SequenceLengthInTicks(playAction->GetAnimationLength());
			pick->UpdateTimeline(noteTick, noteTick + playAnimTicks, 'P');
			pick->UpdateTimeline(noteTick + playAnimTicks, _songLength, stringRoot);

			return;
		}
	}

	UPickPrepareAction* prepareAction = NewObject<UPickPrepareAction>();
	prepareAction->Init(bestPick, bestPick->TimelineStatus(noteTick), stringRoot);
	prepareAction->SetAnimationRate(bestPick->SequenceLengthInTicks(prepareAction->GetAnimationLength()) / (float)(noteTick - bestTick - 1));

	prepareAction->Tick = bestTick;
	actions->Add(prepareAction);

	bestPick->UpdateTimeline(bestTick, noteTick, 'P');

	UPickPlayAction* playAction = NewObject<UPickPlayAction>();
	playAction->Init(bestPick, stringRoot);
	playAction->Tick = noteTick;
	actions->Add(playAction);

	int32 playAnimTicks = bestPick->SequenceLengthInTicks(playAction->GetAnimationLength());
	bestPick->UpdateTimeline(noteTick, noteTick + playAnimTicks, 'P');
	bestPick->UpdateTimeline(noteTick + playAnimTicks, _songLength, stringRoot);

	UE_LOG(VirtusonicError, Warning, TEXT("Could not assign note to pick, set prepare animation scale to %.2f"), 
		bestPick->SequenceLengthInTicks(prepareAction->GetAnimationLength()) / (float)(noteTick - bestTick - 1));
}

void AStringInstrument::GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{

}


