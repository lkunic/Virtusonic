// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StringInstrument.h"

/*
 * Configures the references to string instrument controller components.
 * Audio controller - contains references to audio sources (should be 1 per string).
 * Fret finger controller - contains references to fret fingers
 * Pick controller - contains references to picks
 * String controller - contains references to strings
 */
void AStringInstrument::SetControllerComponents(
	UAudioController* audioController, UFretFingerController* fretFingerController,
	UPickController* pickController, UStringController* stringController)
{
	_audioController = audioController;
	_fretFingerController = fretFingerController;
	_pickController = pickController;
	_stringController = stringController;
}

/*
 * Generates actions for string instruments. 
 * For each note, separate actions are generated for audio, picks, fret fingers and strings.
 */
TArray<UBaseTimelineAction*> AStringInstrument::GenerateActions(TArray<USongNote*> notes)
{
	TArray<UBaseTimelineAction*> actions;
	StringPosition position;

	// A buffer is added to the end of the song so that all animations have time to finish
	_songLength = notes.Last()->GetEndTick() + 4 * _ticksPerQuarter;

	// Initialize the actors (generate the timelines, load animations...)
	InitPicks();
	//InitFretFingers();
	//InitStrings();

	for (int i = 0; i < notes.Num(); i++)
	{
		position = GetStringPositionForNote(notes[i]);

		// Generate actions for the note
		GenerateAudioActions(&actions, notes[i], position);
		GenerateFretFingerActions(&actions, notes[i], position);
		GeneratePickActions(&actions, notes[i], position);
		GenerateStringActions(&actions, notes[i], position);
	}

	// Perform any postprocessing and cleanup on the actors (return to rest positions...)
	CleanupPicks(&actions);
	// CleanupFretFingers(&actions);
	// CleanupStrings(&actions);
	
	return actions;
}

/// VIRTUAL FUNCTIONS ///

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

 /// ///////////////// ///

/*
 * Calculates the string position for the given note.
 * TODO - implement, will probably use DP graph search to find the minimum effort fingering.
 */
StringPosition AStringInstrument::GetStringPositionForNote(USongNote* note)
{
	return *GetPossibleStringPositions(note->GetPitch())[0];
}

/// AUDIO FUNCTIONS ///

/*
 * Generates actions for playing and stopping the audio for the given note.
 */
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

/// FRET FINGER FUNCTIONS ///

void AStringInstrument::GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{

}

/// PICK FUNCTIONS ///

/*
 * Initializes the picks, loads pick animations.
 */
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

/*
 * Makes sure all picks return to the resting position after the last note is played.
 */
void AStringInstrument::CleanupPicks(TArray<UBaseTimelineAction*>* actions)
{
	APick* pick;
	for (int i = 0; i < _pickController->GetPickCount(); i++)
	{
		pick = _pickController->GetPick(i);
		ReturnPickToRest(actions, pick);
	}
}

/*
 * Adds an action for returning the pick to the resting position.
 */
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

/*
 * Generates actions for controlling the picks: preparing, picking, resting.
 */
void AStringInstrument::GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{
	APick* pick;

	TCHAR stringRoot = GetStringRoots()[stringPosition.string];
	int32 noteTick = note->GetStartTick();
	int32 lastTick;

	// If no pick can trivially be selected for playing the note, the pick that has been idle the longest
	// is selected, and the prepare animation is shortened to fit before playing the note.
	APick* bestPick = _pickController->GetPick(0);
	int32 bestTick = _songLength;

	for (int i = 0; i < _pickController->GetPickCount(); i++)
	{
		// Find the best candidate for shortening the prepare animation in case no suitable pick is found
		pick = _pickController->GetPick(i);
		lastTick = pick->GetLastPlayTick();
		if (lastTick < bestTick)
		{
			bestPick = pick;
			bestTick = lastTick;
		}

		if (pick->CanPlayNote(noteTick, stringPosition.string))
		{
			// Check if the pick can be moved back to the resting position before moving back to play this note
			if (pick->CanRestBeforePick(noteTick))
			{
				ReturnPickToRest(actions, pick);
			}

			// Check if the pick needs to be prepared for picking a different string
			if (pick->TimelineStatus(noteTick) != stringRoot)
			{
				UPickPrepareAction* prepareAction = NewObject<UPickPrepareAction>();
				prepareAction->Init(pick, pick->TimelineStatus(noteTick), stringRoot);

				int32 prepareAnimTicks = pick->SequenceLengthInTicks(prepareAction->GetAnimationLength());
				prepareAction->Tick = noteTick - prepareAnimTicks;
				actions->Add(prepareAction);

				pick->UpdateTimeline(noteTick - prepareAnimTicks, noteTick, 'P');
			}

			// Add the actual play animation
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

	// No suitable candidate was found in the first pass, so the prepare animation needs to be shortened (by increasing the animation rate scale)
	UPickPrepareAction* prepareAction = NewObject<UPickPrepareAction>();
	prepareAction->Init(bestPick, bestPick->TimelineStatus(noteTick), stringRoot);
	prepareAction->SetAnimationRate(bestPick->SequenceLengthInTicks(prepareAction->GetAnimationLength()) / (float)(noteTick - bestTick - 1));

	prepareAction->Tick = bestTick;
	actions->Add(prepareAction);

	bestPick->UpdateTimeline(bestTick, noteTick, 'P');

	// The play animation is performed at the original rate
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

/// STRING FUNCTIONS ///

void AStringInstrument::GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition)
{

}


