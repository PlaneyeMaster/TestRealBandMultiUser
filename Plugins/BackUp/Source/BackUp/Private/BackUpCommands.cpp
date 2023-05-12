// Copyright Epic Games, Inc. All Rights Reserved.

#include "BackUpCommands.h"

#define LOCTEXT_NAMESPACE "FBackUpModule"

void FBackUpCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "BackUp", "Custom MultiUser Editor Plugin", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
