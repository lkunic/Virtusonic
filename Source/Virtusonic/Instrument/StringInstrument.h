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

/**
 * 
 */
UCLASS()
class VIRTUSONIC_API AStringInstrument : public AInstrument
{
	GENERATED_BODY()
	
public:
	virtual TArray<UBaseTimelineAction*> GenerateActions(USongNote* note) override;

	void SetControllerComponents(
		UAudioController* audioController, UFretFingerController* fretFingerController,
		UPickController* pickController, UStringController* stringController);

private:
	void GenerateAudioActions(TArray<UBaseTimelineAction*>* actions, USongNote* note);
	void GenerateFretFingerActions(TArray<UBaseTimelineAction*>* actions, USongNote* note);
	void GeneratePickActions(TArray<UBaseTimelineAction*>* actions, USongNote* note);
	void GenerateStringActions(TArray<UBaseTimelineAction*>* actions, USongNote* note);

	UAudioController* _audioController;
	UFretFingerController* _fretFingerController;
	UPickController* _pickController;
	UStringController* _stringController;
};
