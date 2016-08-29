// Copyright (c) Luka Kunic, 2016.

#include "Virtusonic.h"
#include "StartupMenu.h"

TArray<FString> UStartupMenu::GetSongs()
{
	TArray<FString> songNames;

	TArray<FString> fileNames;
	IFileManager::Get().FindFiles(fileNames, *FPaths::GameContentDir().Append("Songs"), TEXT(".mid"));
	
	for (FString file : fileNames)
	{
		songNames.Add(file.LeftChop(4));
	}

	return songNames;
}


