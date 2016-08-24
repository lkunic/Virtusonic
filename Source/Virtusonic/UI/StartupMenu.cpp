// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StartupMenu.h"

TArray<FString> UStartupMenu::GetSongs()
{
	TArray<FString> songNames;

	songNames.Add("CMajor");
	songNames.Add("Gallop");
	songNames.Add("TheChase");
	songNames.Add("TheFlower");
	songNames.Add("TwinkleTwinkle");


	return songNames;
}


