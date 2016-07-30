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
	void SetControllerComponents(
		UAudioController* audioController, UFretFingerController* fretFingerController,
		UPickController* pickController, UStringController* stringController);

	virtual TArray<UBaseTimelineAction*> GenerateActions(TArray<USongNote*> notes) override;

	StringPosition GetStringPositionForNote(USongNote* note);

	virtual TArray<StringPosition*> GetPossibleStringPositions(int32 notePitch);
	virtual FString GetPickAnimationPath();
	virtual FString GetStringRoots();

private:
	void InitPicks();
	void ReturnPickToRest(TArray<UBaseTimelineAction*>* actions, APick* pick);

	void GenerateAudioActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);
	void GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);
	void GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);
	void GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note, StringPosition stringPosition);

	UAudioController* _audioController;
	UFretFingerController* _fretFingerController;
	UPickController* _pickController;
	UStringController* _stringController;
};
