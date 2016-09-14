// Copyright (c) Luka Kunic, 2016.

#pragma once

#include "Instrument/Instrument.h"
#include "Instrument/StringInstrumentFingeringGraph.h"

#include "Audio/AudioController.h"
#include "FretFinger/FretFingerController.h"
#include "Pick/PickController.h"
#include "String/StringController.h"

#include "Song/TimelineActions/General/AudioPlayAction.h"
#include "Song/TimelineActions/General/AudioStopAction.h"
#include "Song/TimelineActions/StringInstrument/FretFingerPrepareAction.h"
#include "Song/TimelineActions/StringInstrument/FretFingerMoveAction.h"
#include "Song/TimelineActions/StringInstrument/FretFingerPressAction.h"
#include "Song/TimelineActions/StringInstrument/FretFingerReleaseAction.h"
#include "Song/TimelineActions/StringInstrument/FretFingerRestAction.h"
#include "Song/TimelineActions/StringInstrument/PickPlayAction.h"
#include "Song/TimelineActions/StringInstrument/PickPrepareAction.h"
#include "Song/TimelineActions/StringInstrument/PickRestAction.h"
#include "Song/TimelineActions/StringInstrument/StringPlayAction.h"
#include "Song/TimelineActions/StringInstrument/StringPressAction.h"
#include "Song/TimelineActions/StringInstrument/StringReleaseAction.h"

#include "StringInstrument.generated.h"

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
		UAudioController *audioController, UFretFingerController *fretFingerController,
		UPickController *pickController, UStringController *stringController);

	// Overriden function for generating timeline actions
	virtual TArray<UBaseTimelineAction*> GenerateActions(const TArray<USongNote*> &notes) override;

	// These functions should be implemented by derived string instrument classes
	virtual TArray<FStringPosition> GetPossibleStringPositions(int8 notePitch);
	virtual TArray<float> GetFretPositions();
	virtual FString GetPickAnimationPath();
	virtual FString GetFretFingerAnimationPath();
	virtual FString GetStringAnimationPath();
	virtual FString GetStringRoots();
	virtual int8 GetFretCount();
	virtual int8 GetStringCount();

private:
	FStringPosition GetStringPositionForNote(int32 noteIndex);
	FFingerboardState GetFingerboardStateForNote(int32 noteIndex);

	// Audio
	void GenerateAudioActions(TArray<UBaseTimelineAction*> &actions, USongNote *note, FStringPosition stringPosition);

	// Fret fingers
	void InitFretFingers();
	void CleanupFretFingers(TArray<UBaseTimelineAction*> &actions);
	void ReturnFretFingerToRest(TArray<UBaseTimelineAction*> &actions, AFretFinger *fretFinger);
	void GenerateFretFingerActions(TArray<UBaseTimelineAction*> &actions, USongNote *note, FStringPosition stringPosition, FFingerboardState fingerboardState);

	// Picks
	void InitPicks();
	void CleanupPicks(TArray<UBaseTimelineAction*> &actions);
	void ReturnPickToRest(TArray<UBaseTimelineAction*> &actions, APick *pick);
	void GeneratePickActions(TArray<UBaseTimelineAction*> &actions, USongNote *note, FStringPosition stringPosition);

	// Strings
	void InitStrings();
	void GenerateStringActions(TArray<UBaseTimelineAction*> &actions, USongNote *note, FStringPosition stringPosition);

	UAudioController *mAudioController;
	UFretFingerController *mFretFingerController;
	UPickController *mPickController;
	UStringController *mStringController;

	UPROPERTY()
	UStringInstrumentFingeringGraph *mFingeringGraph;
};
