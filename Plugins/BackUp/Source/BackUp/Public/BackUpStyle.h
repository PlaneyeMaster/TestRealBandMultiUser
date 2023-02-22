// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FBackUpStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

	static void SetIcon(const FString& StyleName, const FString& ResourcePath);
	static void SetSVGIcon(const FString& StyleName, const FString& ResourcePath);


private:

	static TSharedRef< class FSlateStyleSet > Create();
	static TUniquePtr<FSlateStyleSet> MSStyleInstance;
private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};