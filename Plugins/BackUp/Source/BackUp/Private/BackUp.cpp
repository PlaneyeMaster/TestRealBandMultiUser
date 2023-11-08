// Copyright Epic Games, Inc. All Rights Reserved.

#include "BackUp.h"
#include "BackUpStyle.h"
#include "BackUpCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Http.h"

#include "UI/FRealBandBackUpUIManager.h"

static const FName BackUpTabName("BackUp");

#define LOCTEXT_NAMESPACE "FBackUpModule"

void FBackUpModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FBackUpStyle::Initialize();
	FBackUpStyle::ReloadTextures();

	FBackUpCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FBackUpCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FBackUpModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBackUpModule::RegisterMenus));

	pFRealBandBackUpUIManager = MakeShareable(new FRealBandBackUpUIManager);
	pFRealBandBackUpUIManager->Setup();
	
}

void FBackUpModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FRealBandBackUpUIManager::Instance.IsValid())
	{
		
		FRealBandBackUpUIManager::Instance.Reset();
	}

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FBackUpStyle::Shutdown();

	FBackUpCommands::Unregister();
}

void FBackUpModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FBackUpModule::PluginButtonClicked()")),
							FText::FromString(TEXT("BackUp.cpp"))
					   );
	//FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	//SetupMenuItem()
	FRealBandBackUpUIManager::Initialize();
	//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	//Request->SetVerb("POST");
	//Request->SetURL("https://example.com/backup");
	//Request->SetHeader("Content-Type", "application/json");
	//Request->SetContentAsString("{\"data\":\"example\"}");
	//Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	//	if (bWasSuccessful && Response.IsValid()) {
	//		// Handle the response from the server
	//	}
	//	else {
	//		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to connect"));
	//		// Handle the error
	//	}
	//	});
	//Request->ProcessRequest();
}

void FBackUpModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FBackUpCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginToolsME");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FBackUpCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBackUpModule, BackUp)