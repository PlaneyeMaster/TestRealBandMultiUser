// Copyright Epic Games, Inc. All Rights Reserved.

#include "BackUpStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyle.h"
//#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
//#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/AppStyle.h"


TUniquePtr<FSlateStyleSet> FBackUpStyle::MSStyleInstance;

//#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FBackUpStyle::StyleInstance = nullptr;

FString PluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("RealBandMultiUser"));

//#define TTF_FONT(RelativePath, ...) FSlateFontInfo(FPaths::EngineContentDir() / "Resources" / RelativePath + TEXT(".ttf"), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(FPaths::Combine(TEXT("RealBandMultiUser"), TEXT("Resources"), TEXT("*.otf")), __VA_ARGS__)
//#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FPaths::Combine(TEXT("RealBandImporter"), TEXT("Resources"), TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush( Style->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(20.0f, 20.0f);


void FBackUpStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FBackUpStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FBackUpStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("BackUpStyle"));
	return StyleSetName;
}



TSharedRef< FSlateStyleSet > FBackUpStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("BackUpStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("BackUp")->GetBaseDir() / TEXT("Resources"));
	Style->Set("MyFontStyle", FTextBlockStyle(FTextBlockStyle::GetDefault())

		.SetColorAndOpacity(FSlateColor(FLinearColor(100.4352f, 100.3647f, 100.8862f)))
		.SetFont(TTF_FONT("Pretendard-Black", 40)));

	Style->Set("BackUp.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FBackUpStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FBackUpStyle::Get()
{
	return *StyleInstance;
}

void FBackUpStyle::SetIcon(const FString& StyleName, const FString& ResourcePath)
{
	FSlateStyleSet* Style = MSStyleInstance.Get();

	FString Name("RealBand");
	Name = Name + "." + StyleName;
	Style->Set(*Name, new IMAGE_BRUSH(ResourcePath, Icon40x40));

	//	Name += ".Small";
	//	Style->Set(*Name, new IMAGE_BRUSH(ResourcePath, Icon20x20));

	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

void FBackUpStyle::SetSVGIcon(const FString& StyleName, const FString& ResourcePath)
{
	FSlateStyleSet* Style = MSStyleInstance.Get();

	FString Name("RealBand");
	Name = Name + "." + StyleName;
	Style->Set(*Name, new IMAGE_BRUSH_SVG(ResourcePath, Icon40x40));

	const FSlateBrush* pSlateBrush = Style->GetBrush(FName("RealBand.SaveIcon"));
	if (pSlateBrush)
	{
		FName brushName;
		brushName = pSlateBrush->GetResourceName();
	}

	Name += ".Small";
	Style->Set(*Name, new IMAGE_BRUSH_SVG(ResourcePath, Icon20x20));

	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

