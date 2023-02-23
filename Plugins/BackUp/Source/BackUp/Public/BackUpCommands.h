// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "BackUpStyle.h"

class FBackUpCommands : public TCommands<FBackUpCommands>
{
public:

	FBackUpCommands()
		: TCommands<FBackUpCommands>(TEXT("BackUp"), NSLOCTEXT("Contexts", "BackUp", "BackUp Plugin"), NAME_None, FBackUpStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
