// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StartupMenu.h"

TArray<FString> UStartupMenu::GetSongs()
{
	TArray<FString> songNames;

	songNames.Add("Gallop");
	songNames.Add("TwinkleTwinkle");
	songNames.Add("TheChase");

	return songNames;
}


