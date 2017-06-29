// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "SongFinishedAction.h"

/*
* Initializes the action for finishing the song.
*/
void USongFinishedAction::Init(AVirtusonicGameState *gameState)
{
	mGameState = gameState;
}

/*
* Executes the action changes the game state to finished.
*/
void USongFinishedAction::Execute()
{
	mGameState->bSongFinished = true;
}



