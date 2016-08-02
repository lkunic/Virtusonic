// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Instrument/Instrument.h"

#include "Audio/AudioController.h"
#include "FretFinger/FretFingerController.h"
#include "Pick/PickController.h"
#include "String/StringController.h"

#include "Song/TimelineActions/AudioPlayAction.h"
#include "Song/TimelineActions/AudioStopAction.h"
#include "Song/TimelineActions/FretFingerPrepareAction.h"
#include "Song/TimelineActions/FretFingerPressAction.h"
#include "Song/TimelineActions/FretFingerReleaseAction.h"
#include "Song/TimelineActions/FretFingerRestAction.h"
#include "Song/TimelineActions/PickPlayAction.h"
#include "Song/TimelineActions/PickPrepareAction.h"
#include "Song/TimelineActions/PickRestAction.h"
#include "Song/TimelineActions/StringPlayAction.h"
#include "Song/TimelineActions/StringPressAction.h"
#include "Song/TimelineActions/StringReleaseAction.h"

#include "StringInstrument.generated.h"

typedef struct {
	int32 string;
	int32 fret;
} StringPosition;

/**
 *  Base 'abstract' class for all string instruments (guitar, bass, ukulele...).
 */
UCLASS()
class VIRTUSONIC_API AStringInstrument : public AInstrument
{
	GENERATED_BODY()
	
public:

	// Every string instrument should call this to set the controller references during BeginPlay
	void SetControllerComponents(
		UAudioController* audioController, UFretFingerController* fretFingerController,
		UPickController* pickController, UStringController* stringController);

	// Overriden function for generating timeline actions
	virtual TArray<UBaseTimelineAction*> GenerateActions(TArray<USongNote*> notes) override;

	// These functions should be implemented by derived string instrument classes
	virtual TArray<StringPosition*> GetPossibleStringPositions(int32 notePitch);
	virtual FString GetPickAnimationPath();
	virtual FString GetStringAnimationPath();
	virtual FString GetStringRoots();

private:
	StringPosition GetStringPositionForNote(USongNote* note);

	// Audio
	void GenerateAudioActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);

	// Fret fingers
	void GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);

	// Picks
	void InitPicks();
	void CleanupPicks(TArray<UBaseTimelineAction*>* actions);
	void ReturnPickToRest(TArray<UBaseTimelineAction*>* actions, APick* pick);
	void GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);

	// Strings
	void InitStrings();
	void GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);

	UAudioController* _audioController;
	UFretFingerController* _fretFingerController;
	UPickController* _pickController;
	UStringController* _stringController;
};
