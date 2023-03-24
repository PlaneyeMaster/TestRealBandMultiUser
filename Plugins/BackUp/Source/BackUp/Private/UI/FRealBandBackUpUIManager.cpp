// Copyright RealEye, Inc. All Rights Reserved.

//UI


#include "../UI/FRealBandBackUpUIManager.h"
#include "../Config/RealBandConfig.h"
#include "CoreGlobals.h"
#include "BackUpStyle.h"

#include "Modules/ModuleManager.h"
#include "IMessagingModule.h"
#include "Interfaces/IMainFrameModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Interfaces/IPluginManager.h"
//Editor
#include "LevelEditor.h"

//#include "MultiUserServerModule.h"
#include "IMultiUserServerModule.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "Blutility/Classes/EditorUtilityObject.h"

#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/ConcertSettings.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/IConcertClient.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClient.h"
#include "IConcertClientWorkspace.h"
#include "../../../../Engine/Plugins/Developer\Concert\ConcertSync\ConcertSyncServer\Source\ConcertSyncServer\Public\IConcertSyncServerModule.h"


#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClientModule.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public/IMultiUserClientModule.h"
#include "../../../../Engine/Plugins/Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public/ConcertFrontendStyle.h"
#include "../../../../Engine\Plugins\Developer\Concert\ConcertSync\ConcertSyncCore\Source\ConcertSyncCore\Public\ConcertLocalFileSharingService.h"
#include "../../../../Engine\Plugins\Messaging\UdpMessaging\Source\UdpMessaging\Public\Shared\UdpMessagingSettings.h"
#include "../../../../Engine\Plugins\Messaging\UdpMessaging\Source\UdpMessaging\Private\Transport\UdpMessageTransport.h"
#include "../../../../Engine/Source/Editor/ContentBrowser/Private/SContentBrowser.h"
#include "../../../../Engine/Plugins\Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public\ConcertFrontendUtils.h"
#include "../../../../Engine/Plugins\Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public\Session\Browser\SConcertSessionBrowser.h"
#include "../../../../Engine/Plugins\Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public\Session\Browser\IConcertSessionBrowserController.h"
#include "SessionServices/Public/ISessionServicesModule.h"

//#include "../../../../Engine/Plugins\VirtualProduction\MultiUserTakes\Source\ConcertTakeRecorder\Private\ConcertTakeRecorderMessages.h"

//Git
#include "../../../../Engine/Plugins\Developer\GitSourceControl\Source\GitSourceControl\Private\GitSourceControlModule.h"
#include "../../../../Engine/Plugins\Developer\GitSourceControl\Source\GitSourceControl\Private\GitSourceControlUtils.h"
#include "../../../../Engine\Plugins\Developer\GitSourceControl\Source\GitSourceControl\Private\GitSourceControlSettings.h"


#include "SourceControlHelpers.h"
#include "SourceControl\Public\SourceControlOperations.h"
#include "ISourceControlModule.h"
#include "SourceControlWindows\Public\ISourceControlWindowsModule.h"
#include "FileHelpers.h"
#include "Misc/FileHelper.h"

//C:\Program Files\Epic Games\UE_5.1\Engine\Source\Editor\SourceControlWindows\Public\SourceControlWindows.h
#include "SourceControlWindows\Public\SourceControlWindows.h"

#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
//#include <windows.h>
#include <Lmcons.h>

// For Python 
#include "GenericPlatform/GenericPlatformProcess.h"



DEFINE_LOG_CATEGORY(LogManager);

TSharedPtr<FRealBandBackUpUIManagerImpl> FRealBandBackUpUIManager::Instance;
#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <winreg.h>
#include "Windows/HideWindowsPlatformTypes.h"

//static const FString WindowsRunRegKeyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
//static const FString ListenerAutolaunchRegValueName = "SwitchboardListener";

//FString GetListenerAutolaunchEntry();
//FString GetListenerAutolaunchEntryExecutable();
//bool SetListenerAutolaunchEntry(const FString& NewCommandLine);
//bool RemoveListenerAutolaunchEntry();
#endif

#include <direct.h>

using namespace GitSourceControlUtils;

#define LOCTEXT_NAMESPACE "RealBandBackUP"

namespace RealBandMultiUser {
	static constexpr int64 MaximumNumberOfActivities = 1000;
};

FRealBandBackUpUIManager::~FRealBandBackUpUIManager()
{
	FConcertFrontendStyle::Shutdown();
}


FRealBandBackUpUIManagerImpl::~FRealBandBackUpUIManagerImpl()
{
	pDialogMainWindow.Reset();
	pOverlay.Reset();
	pCanvas.Reset();
	if (FRealBandBackUpUIManager::Instance)
	{
		FRealBandBackUpUIManager::Instance.Reset();
		FRealBandBackUpUIManager::Instance = nullptr;
	}

	// need to disable the source control as there is a known issue of delay in launch of the project if source control is enabled 
	FSourceControlModule& SourceControlModule = FSourceControlModule::Get();
	
	SourceControlModule.SetProvider("None");
	SourceControlModule.UnregisterSourceControlProjectDirDelegate();

	const FString& IniFile = SourceControlHelpers::GetSettingsIni();
	FString SettingsSection = TEXT("GitSourceControl.GitSourceControlSettings");
	

	
}
void FRealBandBackUpUIManagerImpl::Initialize()
{
	// Menu Setup
//	//SetupMenuItem();
	//Create the MainWindow
	CreateWidgetWindow();

}
//
//
//
void FRealBandBackUpUIManager::Initialize()
{
//	
	if (!Instance.IsValid())
    {
//	//	//FRealBandStyle::Initialize();
	    Instance = MakeShareable(new FRealBandBackUpUIManagerImpl());
//	//	FRealBandStyle::SetSVGIcon("SaveIcon", "saveIcon");
//	//	//FRealBandStyle::SetIcon("Icon8", "IconsB");
//	//	FRealBandStyle::SetIcon("Icon8", "Icon40x40");
	}


	//IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
//	if (!MultiUserClientModule.GetClient()->GetConcertClient()->IsStarted())
	{
		FString ProjectFileName = FApp::GetProjectName() + FString(".uproject");
		
		FString ProjectFilePath = FPaths::ProjectDir() / ProjectFileName;
		ProjectFilePath = FPaths::ConvertRelativePathToFull(ProjectFilePath);

		std::wstring currwdir;
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		currwdir = std::wstring(buffer).substr(0, pos);

		bool isSuccess = Instance->InitSourceVersionControl();
		if (!isSuccess)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to Initialize source control. Check the Debug Output"));
			return;
		}
		//Instance->Save();
		Instance->InitMultiUserEditorControls();
		Instance->Initialize();
	}
	
}
//
////bool FRealBandUIManager::DisplayStartWindow()
////{
////	bool bRet = false;
////	if (Instance)
////	{
////		bRet = Instance->RestoreMainWindow();
////	}
////	return bRet;
////}
////
////
void FRealBandBackUpUIManagerImpl::SetupMenuItem()
{
    FBackUpStyle::SetIcon("Logo", "Logo80x80");
	FBackUpStyle::SetIcon("ContextLogo", "Logo32x32");
    //FRealBandStyle::SetSVGIcon("MenuLogo", "QuixelBridgeB");
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LEVELEDITOR_MODULE_NAME);
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, nullptr, 
		                                 FToolBarExtensionDelegate::CreateRaw(this, &FRealBandBackUpUIManagerImpl::FillToolbar));
////	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
////
////	// Adding Bridge entry to Quick Content menu.
////	UToolMenu* AddMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.AddQuickMenu");
////	FToolMenuSection& Section = AddMenu->FindOrAddSection("Content");
////	Section.AddMenuEntry("OpenRealBand",
////		LOCTEXT("OpenRealBand_Label", "RealBandImporter"),
////		LOCTEXT("OpenRealBand_Desc", "Opens the RealBand Importer."),
////		FSlateIcon(FName("RealBandImporterStyle"), "RealBand.MenuLogo"),
////		FUIAction(FExecuteAction::CreateRaw(this, &FRealBandUIManagerImpl::CreateWindow), FCanExecuteAction())
////	).InsertPosition = FToolMenuInsert("ImportContent", EToolMenuInsertType::After);
}
////
void FRealBandBackUpUIManagerImpl::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
    	ToolbarBuilder.BeginSection(TEXT("RealBandMultiUserEditor"));
	    {
    		ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateRaw(this, &FRealBandBackUpUIManagerImpl::CreateWidgetWindow)),
			FName(TEXT("RealBand MultiUser Editor Backup")),
			LOCTEXT("QMSLiveLink_label", "RealBand MultiUser Editor Backup"),
			LOCTEXT("WorldProperties_ToolTipOverride", "Megascans Link"),
			FSlateIcon(FBackUpStyle::GetStyleSetName(), "RealBand.Logo"),
			EUserInterfaceActionType::Button,
			FName(TEXT("RealBand MultiUser Editor Backup"))
		);
    	}
    	ToolbarBuilder.EndSection();
}

//
void FRealBandBackUpUIManagerImpl::CreateWidgetWindow()
{

	FString rUserName = FPlatformProcess::UserName();
	FString repoName = FApp::GetProjectName();
//	bool bIsVisible = false;
	if (!pDialogMainWindow )
	{
		FSlateBrush* CustomBorderBrush = new FSlateBrush();
		CustomBorderBrush->SetResourceObject(FCoreStyle::Get().GetBrush("GenericWhiteBox")->GetResourceObject());
		CustomBorderBrush->ImageSize = FVector2D(0.2f, 0.2f);
		//CustomBorderBrush->Margin = FVector2D(8.2f, 8.2f);


		pDialogMainWindow = SNew(SWindow)
			.Title(FText::FromString("RealBandBackUp MultiUser Session"))
			.ClientSize(FVector2D(1100, 400))
			.SupportsMaximize(false)
			.SupportsMinimize(false)
			.SizingRule(ESizingRule::UserSized)
			.AutoCenter(EAutoCenter::PreferredWorkArea)
			.SupportsMaximize(true)
			.SupportsMinimize(true)
			[
				SAssignNew(pOverlay, SOverlay)
        		+ SOverlay::Slot()
		         .VAlign(VAlign_Fill)
			     .HAlign(HAlign_Fill)			
			    [
				  SNew(SBox)
				 .HAlign(HAlign_Fill)
			     .VAlign(VAlign_Fill)
			      [
				     SAssignNew(pCanvas, SCanvas)
					 + SCanvas::Slot()
			          .HAlign(HAlign_Fill)
			          .VAlign(VAlign_Fill)
			          .Size(FVector2D(120.0f, 100.0f))
			          .Position(FVector2D(30.0f, 45.0f))
			          [
				         SNew(STextBlock)
				         .Text(FText::FromString("Current User"))
			             //.Font(FCoreStyle::Get().GetFontStyle(TEXT("PropertyWindow.NormalFont")))
			             .TextStyle(FAppStyle::Get(), "NormalText")
			             //.TextStyle(FTextBlockStyle)
                         ////			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)
			          ]
		             + SCanvas::Slot()
			           .HAlign(HAlign_Fill)
			           .VAlign(VAlign_Fill)
			           .Size(FVector2D(120.0f, 100.0f))
			           .Position(FVector2D(250.0f, 45.0f))
			           [
				          SNew(STextBlock)
						  .Text(FText::FromString(rUserName))
			              .TextStyle(FAppStyle::Get(), "NormalText")
			             //.TextStyle(FTextBlockStyle)
			            
			            ]
					 + SCanvas::Slot()
		               .HAlign(HAlign_Fill)
		               .VAlign(VAlign_Fill)
		               .Size(FVector2D(120.0f, 100.0f))
		               .Position(FVector2D(30.0f, 85.0f))
		               [
			               SNew(STextBlock)
			               .Text(FText::FromString("Repository Name"))
		                   //.Font(FCoreStyle::Get().GetFontStyle(TEXT("PropertyWindow.NormalFont")))
		                   .TextStyle(FAppStyle::Get(), "NormalText")
		                   
		                   //.TextStyle(FTextBlockStyle)
		////			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)

		               ]
	                + SCanvas::Slot()
		              .HAlign(HAlign_Fill)
		              .VAlign(VAlign_Fill)
		              .Size(FVector2D(400.0f, 20.0f)) // 170, 20
		              .Position(FVector2D(250.0f, 80.0f))
		              [
						  SNew(SBorder)
						  .BorderImage(CustomBorderBrush)
						  .BorderBackgroundColor(FLinearColor::White)
						 // .HAlign(HAlign_Center)
						 // .VAlign(VAlign_Center)
						  .Padding(FMargin(0.2f))
						  
						[
			              SNew(SEditableText)
						  .ColorAndOpacity(FLinearColor::Black)
						  .Font(FAppStyle::GetFontStyle("Regular"))
						  .Text(FText::FromString(remoteUrl))
						  .MinDesiredWidth(250.0f)
						  
						]
						  //.Style
		//.Font(FCoreStyle::Get().GetFontStyle(TEXT("PropertyWindow.NormalFont")))
		            //      .TextStyle(FAppStyle::Get(), "NormalText")

		//.TextStyle(FTextBlockStyle)

		              ]

			        // + SCanvas::Slot()
			        //  .HAlign(HAlign_Fill)
			        //  .VAlign(VAlign_Fill)
			        //  .Size(FVector2D(120.0f, 50.0f))
			        //  .Position(FVector2D(30.0f, 180.0f))
			        //  [
				       // SNew(SButton)
				       //.HAlign(HAlign_Center)
			        //   .VAlign(VAlign_Center)
			        //   .Text(FText::FromString("Start Backup"))

			        //   ]
					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(120.0f, 50.0f))
						 .Position(FVector2D(250.0f, 180.0f))
						 [
							 SAssignNew(pJoinBtn, SButton)
							// SNew(SButton)
							 .HAlign(HAlign_Center)
						 .VAlign(VAlign_Center)
						 .Text(FText::FromString("JOIN"))
						 .OnClicked(this, &FRealBandBackUpUIManagerImpl::JoinSession)

						 ]

					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(70.0f, 40.0f))
						 .Position(FVector2D(660.0f, 68.0f))
						 [
							 SNew(SButton)
							 .HAlign(HAlign_Center)
						     .VAlign(VAlign_Center)
						     .Text(FText::FromString("Sync"))
						     .OnClicked(this, &FRealBandBackUpUIManagerImpl::Sync)

						 ]
					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(70.0f, 40.0f))
						 .Position(FVector2D(760.0f, 68.0f))
						 [
							 SNew(SButton)
							 .HAlign(HAlign_Center)
						 .VAlign(VAlign_Center)
						 .Text(FText::FromString("Save"))
						 .OnClicked(this, &FRealBandBackUpUIManagerImpl::Save)

						 ]
		          ]

			   ]

			];


		FSlateApplication::Get().AddWindow(pDialogMainWindow.ToSharedRef());
		
		TSharedRef<FSlateAccessibleMessageHandler> MessageHandler = FSlateApplication::Get().GetAccessibleMessageHandler();
		
////		//FSlateApplication::Get().OnResizingWindow(pDialogMainWindow)
////
////	//	pSettingsWindow->SetVisibility(EVisibility::Hidden);
////		pApplyButton->SetVisibility(EVisibility::Hidden);
////
////		if(GetAssetViewCount() > 0)
////		    pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
////		else
////			pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);
////
////		pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
////		
////		
////		pRealBandImportSettings->pSettingsWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
////			{
////				pImport->SetVisibility(EVisibility::Visible);
////				pRealBandImportSettings->SetVisibility(EVisibility::Hidden);
////				pApplyButton->SetVisibility(EVisibility::Hidden);
////				pDialogMainWindow->BringToFront(true);;
////				
////			}));
////
	}
	else
	{
	    UE_LOG(LogTemp, Display, TEXT("%hs: ==Main Dialog Window =="), __FUNCTION__);
	    pDialogMainWindow->BringToFront();
		pDialogMainWindow->SetVisibility(EVisibility::Visible);
////		if (!pDialogMainWindow->IsVisible())
////		{
////			pDialogMainWindow->SetVisibility(EVisibility::All);
////			pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
////			pRealBandImportSettings->SetVisibility(EVisibility::Visible);
////		}
    }
//////	pDialogMainWindow->SetOnWindowClosed(FRequestDestroyWindowOverride::CreateSP(this, &FRealBandUIManagerImpl::OnDialogClosed));
////
////
////
////    if (pRealBandImportSettings && GetAssetViewCount() == 0)
////    {
////	    pRealBandImportSettings->SetVisibility(EVisibility::Visible);
////		pApplyButton->SetVisibility(EVisibility::Visible);
////		pImport->SetVisibility(EVisibility::Hidden);
////	}
////
////	pDialogMainWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
////		{
////			pRealBandImportSettings->SaveSettings();
////			//pDialogMainWindow->SetVisibility(EVisibility::Collapsed);
////			pDialogMainWindow.Reset();
////		//	pFRealBandAssetImporter.Reset();
////			//FRealBandUIManager::Instance.Reset();
////		}));
//
}


void FRealBandBackUpUIManagerImpl::InitMultiUserEditorControls()
{

//	if (pDialogMainWindow)
	{
		//FString Role = TEXT("MultiUser");

		// check for Host 
	
		// UDP settings for client 
		FConfigFile* EngineConfig = GConfig ? GConfig->FindConfigFileWithBaseName(FName(TEXT("Engine"))) : nullptr;
		if (EngineConfig)
		{
			TArray<FString> Settings;
			FString Setting;
			// Unicast endpoint setting
			EngineConfig->GetString(TEXT("/Script/UdpMessaging.UdpMessagingSettings"), TEXT("UnicastEndpoint"), Setting);
			if (Setting != "0.0.0.0:0" && !Setting.IsEmpty())
			{
				//check if its the same as my hostname 
				FString hostName = FString(FPlatformProcess::ComputerName());
			
				ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();

				bool bCanBindAll;
				TSharedPtr<class FInternetAddr> HostAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
				uint32 HostIp = 0;
				HostAddr->GetIp(HostIp); // Will return in host order
				FString iAddr = HostAddr->ToString(true);
				if (iAddr == Setting)
				{
					isHostMachine = true;
					UE_LOG(LogTemp, Display, TEXT("%hs: ==HOST MACHINE =="), __FUNCTION__);
				}
				else
				{
					isHostMachine = false;
					UE_LOG(LogTemp, Display, TEXT("%hs: ==CLIENT MACHINE =="), __FUNCTION__);
				}

			}

		}

		FConcertFrontendStyle::Initialize();
		IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
		
		if (MultiUserClientModule.IsConcertServerRunning())
		{
			UE_LOG(LogTemp, Display, TEXT("%hs: ==The ConcertServer is discovered. Connect the default client=="), __FUNCTION__);
			const TSharedPtr<IConcertSyncClient> ConcertSyncClient = IConcertSyncClientModule::Get().GetClient(TEXT("MultiUser"));
			if (!ConcertSyncClient)
			{
				UE_LOG(LogTemp, Error, TEXT("%hs: The ConcertSyncClient could not be found. Please check the output log for errors and try again."), __FUNCTION__);
			}

			MultiUserClientModule.OpenBrowser();
			MultiUserClientModule.DefaultConnect();

			pURealBandConfig = GetMutableDefault<URealBandConfig>();
			if (pURealBandConfig && !pConcertSyncClient)
			{
				pConcertSyncClient = pURealBandConfig->CreateConcertClient();
				TArray<FConcertServerInfo> ServerList = pConcertSyncClient->GetConcertClient()->GetKnownServers();
				//		TArray<FConcertServerInfo> ServerList = MultiUserClientModule.GetClient()->GetConcertClient()->GetKnownServers();
				// Need to differnciate between a client ans server 
				if (ServerList.Num() > 0)
				{
					for (FConcertServerInfo serverInfo : ServerList)
					{
						const FGuid TServerId = serverInfo.AdminEndpointId;
						MultiUserClientModule.GetClient()->GetConcertClient()->GetServerSessions(TServerId)
							.Next([](FConcertAdmin_GetAllSessionsResponse Response)
								{
									if (Response.ResponseCode == EConcertResponseCode::Success)
									{
										for (const FConcertSessionInfo& SessionInfo : Response.LiveSessions)
										{
											IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
											if (MultiUserClientModule.GetClient()->GetConcertClient()->IsOwnerOf(SessionInfo))
											{
												UE_LOG(LogTemp, Log, TEXT("=======Client is the owner =============="));
											}
											else
											{
												UE_LOG(LogTemp, Log, TEXT("=======Server launched on  other machine =============="));
											}
										}
									}
								});
					}

				}

			}
		}
		//FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		
		
		pURealBandConfig = GetMutableDefault<URealBandConfig>();
		if (pURealBandConfig )
		{
			if (isHostMachine)
			{
				UE_LOG(LogTemp, Log, TEXT("=======HOST machine ...Launch Concert Server =============="));
				pURealBandConfig->FindOrLaunchConcertServer();
				pConcertSyncClient = pURealBandConfig->GetBackupClient();
			}
			else
			{
				pConcertSyncClient = MultiUserClientModule.GetClient();
				if (pConcertSyncClient->GetConcertClient()->GetKnownServers().Num() > 0)
				{
					UE_LOG(LogTemp, Log, TEXT("=======Known Servers for Client Found=============="));
					UConcertClientConfig* ClientConfig = NewObject<UConcertClientConfig>();
					ClientConfig->bIsHeadless = true;
					ClientConfig->bInstallEditorToolbarButton = true;
					ClientConfig->bAutoConnect = true;
					ClientConfig->DefaultServerURL = FApp::GetProjectName();;
					//ClientConfig->DefaultSessionName = BackupSessionName;
					//ClientConfig->DefaultSaveSessionAs = BackupSessionName;
					ClientConfig->ClientSettings.AvatarColor = FLinearColor(1.0, 0.0, 0.0);
					ClientConfig->ClientSettings.DisplayName = FPlatformProcess::UserName();
					ClientConfig->EndpointSettings.RemoteEndpointTimeoutSeconds = 0; // Ensure the endpoints never time out (and are kept alive automatically by Concert).
					//ClientConfig->ClientSettings.ClientAuthenticationKey = BackupServerName; // The server adds its own server name to the list of authorized client keys, use that key to authorize this client on the server.
					pConcertSyncClient->GetConcertClient()->Configure(ClientConfig);
				}
				pConcertSyncClient->GetConcertClient()->Startup();
				pConcertSyncClient->GetConcertClient()->StartDiscovery();
				//pConcertSyncClient = pURealBandConfig->CreateConcertClient();
				UE_LOG(LogTemp, Log, TEXT("=======Client machine ...=============="));
			}
			//pConcertSyncClient = pURealBandConfig->GetBackupClient();
			pConcertSyncClient->GetConcertClient()->OnKnownServersUpdated().AddRaw(
					this, &FRealBandBackUpUIManagerImpl::OnServersAssumedReady);

			pConcertSyncClient->GetConcertClient()->OnSessionConnectionChanged().AddSP(this, &FRealBandBackUpUIManagerImpl::HandleSessionConnectionChanged);
			

			IConcertSyncClientModule::Get().OnClientCreated().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleConcertSyncClientCreated);
			for (TSharedRef<IConcertSyncClient> Client : IConcertSyncClientModule::Get().GetClients())
			{
				
				Client->OnSyncSessionStartup().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleSyncSessionStartup);
				Client->GetConcertClient()->OnSessionConnectionChanged().AddRaw(this, &FRealBandBackUpUIManagerImpl::OnSessionConnectionChanged);
					//Client->OnSyncSessionShutdown().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleSyncSessionShutdown);
			}

		}
		
	}
}


void FRealBandBackUpUIManagerImpl::OnSessionConnectionChanged(IConcertClientSession& InSession, EConcertConnectionStatus ConnectionStatus)
{
	switch (ConnectionStatus)
	{
	    case EConcertConnectionStatus::Connected:
		     pJoinBtn->SetEnabled(false);
			 if (pConcertSyncClient)
			 {
				 const TSharedPtr<IConcertClientWorkspace> WorkspacePtr = pConcertSyncClient->GetWorkspace();
				 if (WorkspacePtr)
				 {
					WorkspacePtr->OnActivityAddedOrUpdated().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleActivityAddedOrUpdated);
				 }
			 }
		     break;
		case EConcertConnectionStatus::Disconnected:
			 pJoinBtn->SetEnabled(true);
			 break;
     
	}
}


void FRealBandBackUpUIManagerImpl::HandleActivityAddedOrUpdated(const FConcertClientInfo& InClientInfo, 
	                                                            const FConcertSyncActivity& InActivity, 
	                                                             const FStructOnScope& InActivitySummary)
{
	const TSharedPtr<IConcertClientWorkspace> WorkspacePtr = pConcertSyncClient->GetWorkspace();
	if (WorkspacePtr)
	{
		const int64 LastActivityId = WorkspacePtr->GetLastActivityId();
		const int64 FirstActivityIdToFetch = FMath::Max<int64>(1, LastActivityId - RealBandMultiUser::MaximumNumberOfActivities);

		TMap<FGuid, FConcertClientInfo>   EndpointClientInfoMap;
		TArray<FConcertSessionActivity> FetchedActivities;
		WorkspacePtr->GetActivities(FirstActivityIdToFetch, 100, EndpointClientInfoMap, FetchedActivities);
		FString userName = FPlatformProcess::UserName();
		FString SessionId = FString::Printf(TEXT("%s"), *pConcertSyncClient->GetWorkspace()->GetSession().GetId().ToString());
		FString SummarySessionFile = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + FApp::GetProjectName() + FString("_") + SessionId;
		TArray<FString> SummaryStrings;
		for (FConcertSessionActivity& activity : FetchedActivities)
		{
			FString eventDateTime = activity.Activity.EventTime.ToString(TEXT("%d.%m.%Y %H:%M:%S :"));
			//activity.ActivitySummary->ToDisplayText()
			FText Summary = activity.ActivitySummary->ToDisplayText(FText::FromString(userName));

			SummaryStrings.Add(eventDateTime + Summary.ToString());
		}

		if (!FFileHelper::SaveStringArrayToFile(SummaryStrings, *SummarySessionFile))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save session Activities"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Workspace not found"));
	}
}


void FRealBandBackUpUIManagerImpl::HandleSessionConnectionChanged(IConcertClientSession& InSession,
	                                                               EConcertConnectionStatus ConnectionStatus)
{
	int debug = 1;

	if (ConnectionStatus == EConcertConnectionStatus::Disconnected)
	{
		// Get the source control module
		ISourceControlModule& SourceControlModule = FModuleManager::LoadModuleChecked<ISourceControlModule>("SourceControl");

		// Set the provider to "none"
		//SourceControlModule.GetProvider().Execute<FNullSourceControlCommand>(FNullSourceControlCommandArgs());

		// Save the provider setting
		SourceControlModule.SetProvider(TEXT("None"));
	}

	

}


void FRealBandBackUpUIManagerImpl::OnServersAssumedReady()
{

	if (!ServerId.IsValid() )
	{
		IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
	
		TArray<FConcertServerInfo> ServerList = pConcertSyncClient->GetConcertClient()->GetKnownServers();
		//FGuid ServerId;
		for (FConcertServerInfo serverInst : ServerList)
		{
			FString serverIdent = FApp::GetProjectName();
			FString serverIdentTemp = serverIdent.ToLower();
			FString serverName = serverInst.ServerName.ToLower();
			//   if (!serverInst.ServerName.Contains("RealBand",ESearchCase::IgnoreCase))
			FString sName = serverInst.ServerName;
			bool bStatus = serverInst.ServerName.Contains(serverIdentTemp, ESearchCase::IgnoreCase);
			if (serverName.Contains(serverIdentTemp))
			{
				ServerId = serverInst.AdminEndpointId;
				FConcertCreateSessionArgs   sessionArgs;
				sessionArgs.ArchiveNameOverride = "m_backup";
				sessionArgs.SessionName = "RealBandMultiUser";

				if (MultiUserClientModule.IsConcertServerRunning())
				{
				//	pConcertSyncClient->GetConcertClient()->OnSessionConnectionChanged().AddSP(this, &FRealBandBackUpUIManagerImpl::HandleSessionConnectionChanged);

					pConcertSyncClient->GetConcertClient()->GetServerSessions(ServerId).
						Next([iConcertSyncClient = pConcertSyncClient,iServerId = ServerId](FConcertAdmin_GetAllSessionsResponse Response)
						{
							if (Response.ResponseCode == EConcertResponseCode::Success)
							{
								for (const FConcertSessionInfo& SessionInfo : Response.LiveSessions)
								{
									if (iConcertSyncClient->GetConcertClient()->GetSessionConnectionStatus() ==
										EConcertConnectionStatus::Disconnected)
									{
										iConcertSyncClient->GetConcertClient()->StartAutoConnect();
										TFuture<EConcertResponseCode> Response = 
											        iConcertSyncClient->GetConcertClient()->JoinSession(iServerId, SessionInfo.SessionId);

										//JoinSession(iServerId, SessionInfo.SessionId);
									//	if (Response.Get() == EConcertResponseCode::Success)
										{
											UE_LOG(LogTemp, Log, TEXT("=======JOINED EXISTING SESSION =============="));
										}

										IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
										if (MultiUserClientModule.GetClient()->GetConcertClient()->IsOwnerOf(SessionInfo))
										{
											//	iConcertSyncClient->GetConcertClient()->GetCurrentSession()->Connect();
											UE_LOG(LogTemp, Log, TEXT("=======Client is the owner =============="));
										}
										else
										{
											UE_LOG(LogTemp, Log, TEXT("=======Server launched on  other machine =============="));
										}
									}
								}


								

							}
						});
					//
				}
				else
				{
								
					/// <summary>
					/// 
					/// </summary>
					
					
					if (isHostMachine)
					{
						UE_LOG(LogTemp, Log, TEXT("======= Create Session for the Server  =============="));
						pConcertSyncClient->GetConcertClient()->CreateSession(ServerId, sessionArgs);
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("======= Join Session for the known Server  =============="));
						pConcertSyncClient = MultiUserClientModule.GetClient();
						//TODO: Set the color for users to Green except the host client which is red
						const UConcertClientConfig* pClientConfig = pConcertSyncClient->GetConcertClient()->GetConfiguration();
						UConcertClientConfig* ClientConfig = NewObject<UConcertClientConfig>();
						ClientConfig->ClientSettings = pClientConfig->ClientSettings;
						ClientConfig->ClientSettings.AvatarColor = FLinearColor(0.0, 1.0, 0.0);
						ClientConfig->ClientSettings.DisplayName = FPlatformProcess::UserName();
						pConcertSyncClient->GetConcertClient()->Configure(ClientConfig);

						pConcertSyncClient->GetConcertClient()->GetServerSessions(ServerId).
							Next([iConcertSyncClient = pConcertSyncClient, iServerId = ServerId](FConcertAdmin_GetAllSessionsResponse Response)
							{
									if (Response.ResponseCode == EConcertResponseCode::Success)
									{
										for (const FConcertSessionInfo& SessionInfo : Response.LiveSessions)
										{
											if (iConcertSyncClient->GetConcertClient()->GetSessionConnectionStatus() ==
												EConcertConnectionStatus::Disconnected)
											{
											//	iConcertSyncClient->GetConcertClient()->StartAutoConnect();
												iConcertSyncClient->GetConcertClient()->JoinSession(iServerId, SessionInfo.SessionId);
												UE_LOG(LogTemp, Log, TEXT("=======JOINED EXISTING SESSION =============="));
												IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
												if (MultiUserClientModule.GetClient()->GetConcertClient()->IsOwnerOf(SessionInfo))
												{
													//	iConcertSyncClient->GetConcertClient()->GetCurrentSession()->Connect();
													UE_LOG(LogTemp, Log, TEXT("=======Client is the owner =============="));
												}
												else
												{
													UE_LOG(LogTemp, Log, TEXT("=======Server launched on  other machine =============="));
												}

											    

											}
										}


									}
							});


					}
				}

				
			}
			
		}
	}
	
}

void FRealBandBackUpUIManagerImpl::HandleSyncSessionStartup(const IConcertSyncClient* SyncClient)
{
	if (pConcertSyncClient && ServerId.IsValid())
	{
		if (pConcertSyncClient->GetConcertClient()->GetCurrentSession()->GetConnectionStatus() == EConcertConnectionStatus::Connected)
		{
			;
			int mdebug = 1;
			int ndebug = 2;
		}
		if (TSharedPtr<IConcertClientWorkspace> Workspace = pConcertSyncClient ? pConcertSyncClient->GetWorkspace() : TSharedPtr<IConcertClientWorkspace>())
		{
			Workspace->SetIgnoreOnRestoreFlagForEmittedActivities(false);
			Workspace->GetSession().Connect();
		}
		
	}
}

void FRealBandBackUpUIManagerImpl::HandleConcertSyncClientCreated(TSharedRef<IConcertSyncClient> Client)
{
	TArray<TSharedPtr<FConcertSessionClientInfo>> Clients;
	FConcertSessionClientInfo objClientInfo;
	/*objClientInfo.ClientEndpointId = Client->GetConcertClient()->GetCurrentSession()->GetSessionServerEndpointId();
	objClientInfo.ClientInfo = Client->GetConcertClient()->GetClientInfo();

	TSharedPtr<FConcertSessionClientInfo> ClientSharedObj = MakeShared<FConcertSessionClientInfo>(objClientInfo);
	Clients.Push(ClientSharedObj);

	TArray<TSharedPtr<FConcertSessionClientInfo>> LatestClientPtrs = Clients;

	ConcertFrontendUtils::SyncArraysByPredicate(Clients, MoveTemp(LatestClientPtrs), [](const TSharedPtr<FConcertSessionClientInfo>& ClientToFind)
		{
			return [ClientToFind](const TSharedPtr<FConcertSessionClientInfo>& PotentialClient)
			{
				return PotentialClient->ClientEndpointId == ClientToFind->ClientEndpointId;
			};
		});*/
	
}


FReply FRealBandBackUpUIManagerImpl::Sync()
{
	FGitSourceControlModule& GitSourceControl = FModuleManager::LoadModuleChecked<FGitSourceControlModule>("GitSourceControl");
	
	FName SourceControlFeatureName("SourceControl");
	TArray<ISourceControlProvider*> Providers = IModularFeatures::Get().GetModularFeatureImplementations<ISourceControlProvider>(SourceControlFeatureName);
	for (auto It(Providers.CreateIterator()); It; It++)
	{
		ISourceControlProvider* Provider = *It;
		FString sProviderName = Provider->GetName().GetPlainNameString();
		if (sProviderName == "None")
		{
			int debug = 1;
		}
	}

	//TArray<FString> FileArray;
	//TArray<FString> outFileArray;

	//FString FullPath = FPaths::ProjectDir();
	//if (FullPath.EndsWith(TEXT("/")))
	//{
	//	FullPath.RemoveAt(FullPath.Len() - 1, 1);
	//}

	//FileArray.Add(FPaths::ConvertRelativePathToFull(FullPath));

	//bool bSuccess = false;
	//ISourceControlProvider& SourceControlProvider = ISourceControlModule::Get().GetProvider();
	//SourceControlProvider.Init();
	//TSharedRef<FGetFileList, ESPMode::ThreadSafe> Operation = ISourceControlOperation::Create<FGetFileList>();
	//
	//ECommandResult::Type ResultFile = SourceControlProvider.Execute(Operation, FileArray, EConcurrency::Synchronous);
	//bSuccess = (ResultFile == ECommandResult::Succeeded);
	//if (bSuccess)
	//{
	//	outFileArray = Operation->GetFilesList();
	//	TSharedRef<FCheckIn, ESPMode::ThreadSafe> CheckInOperation = ISourceControlOperation::Create<FCheckIn>();
	//	GitSourceControl.GetProvider().Execute(CheckInOperation);
	//}
	
	FSourceControlModule& SourceControlModule = FSourceControlModule::Get();
	if (SourceControlModule.IsEnabled())
	{
		FString gitBinaryPath;
		GetGitBinaryPath(gitBinaryPath);
		std::string myString = TCHAR_TO_UTF8(*gitBinaryPath);
		myString = "\"" + myString + "\\cmd\\git\"";
		std::string params = "pull";
		std::string fullCommand = myString + " " + params;

		FString FullPath = FPaths::ProjectDir();
		if (_chdir(TCHAR_TO_UTF8(*FullPath)) == -1) {
			UE_LOG(LogTemp, Error, TEXT("Failed to change directory to: %s"), TCHAR_TO_UTF8(*FullPath));
			return FReply::Handled();
		}

		//ISourceControlModule& SourceControlModule = ISourceControlModule::Get();

		// Get the current provider
		//ISourceControlProvider & SourceControlProvider = SourceControlModule.GetProvider();
		
		
		// Get the current Git branch name
		FString BranchName;
		//SourceControlProvider->Execute(TEXT("CURRENT_BRANCH"), BranchName, EConcurrency::Asynchronous);


		//FString BranchName;
		//SourceControlModule.GetProvider().Execute(TEXT("CURRENT_BRANCH"), BranchName, EConcurrency::Asynchronous);


		int result = system(fullCommand.c_str());
		if (result != 0)
		{
			std::cerr << "git pull	failed with exit code " << result << std::endl;
				UE_LOG(LogTemp, Error, TEXT("Failed to pull changes from repository...try manually "));
			return FReply::Handled();
		}
	}

//	TArray<FString> AllFiles = SourceControlHelpers::PackageFilenames(InPackageNames);
	
	//CheckInOperation->SetDescription(L"Commit");
	
	
	// TODO: At the moment there is no implementation to find local files modified . Sync might fail if some files are 
	//       checked out . They need to be manually commited/stashed/discarded before sync can work 
	//FString FullPath = FPaths::ProjectDir();
	//if (FullPath.EndsWith(TEXT("/")))
	//{
	//	FullPath.RemoveAt(FullPath.Len() - 1, 1);
	//}

	//TArray<FSourceControlChangelistRef> Changelists = GitSourceControl.GetProvider().GetChangelists(EStateCacheUsage::ForceUpdate);
	//FString texStatus = GitSourceControl.GetProvider().GetNumLocalChanges();
	//{
	//	UE_LOG(LogTemp, Log, TEXT("=======Commit the local changes =============="));
	//}

	//TArray<FString> FileArray;
	//FileArray.Add(FPaths::ConvertRelativePathToFull(FullPath));
	//TSharedRef<FGetFileList, ESPMode::ThreadSafe> FileListOperation = ISourceControlOperation::Create<FGetFileList>();
	//ECommandResult::Type ResultOp = GitSourceControl.GetProvider().Execute(FileListOperation, FileArray, EConcurrency::Synchronous);
	//bool bSuccess = (ResultOp == ECommandResult::Succeeded);
	//

	
	//TSharedRef<FUpdateStatus, ESPMode::ThreadSafe> SyncOperation = ISourceControlOperation::Create<FUpdateStatus>();
	//
	//ECommandResult::Type Result = GitSourceControl.GetProvider().Execute(SyncOperation,
	//	                          TArray<FString>(), EConcurrency::Synchronous);
	//
	//if (Result == ECommandResult::Succeeded)
	//{
	//	const FText NotificationText = FText::Format(
	//		LOCTEXT("SourceControlMenu_Success", "Operation Sucess: {1} operation "),
	//		FText::FromName(FName("Sync"))
	//	);
	//	FNotificationInfo Info(NotificationText);
	//	Info.ExpireDuration = 8.0f;
	//	FSlateNotificationManager::Get().AddNotification(Info);
	//	UE_LOG(LogTemp, Display, TEXT("%s"), *NotificationText.ToString());
	//}
	//else
	//{
	//	// Report failure with a notification 
	//	const FText NotificationText = FText::Format(
	//		LOCTEXT("SourceControlMenu_Failure", "Error: {0} operation failed!"),
	//		FText::FromName(FName("Sync"))
	//	);
	//	
	//	FNotificationInfo Info(NotificationText);
	//	Info.ExpireDuration = 8.0f;
	//	FSlateNotificationManager::Get().AddNotification(Info);
	//	UE_LOG(LogTemp, Error, TEXT("%s"), *NotificationText.ToString());
	//	
	//}

	return FReply::Handled();
}


FReply FRealBandBackUpUIManagerImpl::Save()
{
	FGitSourceControlModule& GitSourceControl = FModuleManager::LoadModuleChecked<FGitSourceControlModule>("GitSourceControl");
	TArray<FString> Files;
	bool bPromptUserToSave = true; // Ask user if they want to save the unsaved data
	bool bSaveMapPackages = true;
	bool bSaveContentPackages = true;
	if (FEditorFileUtils::SaveDirtyPackages(bPromptUserToSave, bSaveMapPackages, bSaveContentPackages) == false)
	{
		// The user pressed cancel. Abort the import so the user doesn't lose any changes
		UE_LOG(LogTemp, Display, TEXT("The user pressed cancel"));
		//return FReply::Handled();
	}

	FSourceControlModule& SourceControlModule = FSourceControlModule::Get();
	if (SourceControlModule.IsEnabled())
	{

		//TArray<FString> Filenames = FSourceControlWindows::GetSourceControlLocations(/*bContentOnly*/true);
		//TArray<UPackage*> PackagesToSave;
		//TMap<FString, FSourceControlStatePtr> PackageStates;
		//FEditorFileUtils::FindAllSubmittablePackageFiles(PackageStates, true);
		//FEditorFileUtils::FindAllPackageFiles(Filenames);
		//FEditorFileUtils::FindAllSubmittableProjectFiles(PackageStates);
		//   
		//FSourceControlWindows::ChoosePackagesToCheckIn();

		//TArray<FString> FilesToSubmit;
		//for (TMap<FString, FSourceControlStatePtr>::TConstIterator It(PackageStates); It; ++It)
		//{
		//	FilesToSubmit.Add(FPaths::ConvertRelativePathToFull(It.Key()));
		//}


		//	FCheckinResultInfo ResultInfo;
		//	FSourceControlWindows::PromptForCheckin(ResultInfo, FilesToSubmit);
			
			//FString gitBinary = GitSourceControlUtils::FindGitBinaryPath();
		//	if (ResultInfo.Result == ECommandResult::Succeeded)
				
		FString gitBinaryPath;
		GetGitBinaryPath(gitBinaryPath);
		std::string myString = TCHAR_TO_UTF8(*gitBinaryPath);
		myString = "\"" + myString + "\\cmd\\git\"";
		
		FString FullPath = FPaths::ProjectDir();
		if (_chdir(TCHAR_TO_UTF8(*FullPath)) == -1) {
		    UE_LOG(LogTemp, Error, TEXT("Failed to change directory to: %s"), TCHAR_TO_UTF8(*FullPath));
			return FReply::Handled();
		}
		// Get the branch 
		FString BranchName;
		FString gArguments = " rev-parse --abbrev-ref HEAD";
		FString Command = FString(myString.c_str()); //+ " " + gArguments;
		FString Result;
		int32 ReturnCode = -1;
		FString stdOut;
		FString Error;
		//FPlatformProcess::ExecProcess(TEXT("/bin/sh"), *Command, &Result);
		FPlatformProcess::ExecProcess(*Command, *gArguments, &ReturnCode, &stdOut, &Error);
		BranchName = stdOut.TrimEnd().TrimStart();

		// commit the changes
		
		//std::string commitCmd = "commit . -m \"Assets Updated\"";
		std::string commitCmd = "commit ";
		commitCmd.append(".");
		commitCmd.append(" -m ");
		commitCmd.append("\"");
		commitCmd.append("Assets Updated");
		commitCmd.append("\"");
		
		Command = FString(myString.c_str());
		gArguments = " commit . -m ";
		gArguments.Append("\"");
		gArguments.Append("Assets Update");
		gArguments.Append("\"");
		FPlatformProcess::ExecProcess(*Command, *gArguments, &ReturnCode, &stdOut, &Error);
		//int result = system(FullCommitCommand.c_str());
		if (ReturnCode != 0)
		{
			std::cerr << "git commit failed with exit code " << ReturnCode << std::endl;
			UE_LOG(LogTemp, Error, TEXT("Failed to push changes to repository...try manually "));
			//return FReply::Handled();
			
		}
		// push the changes
		std::string params = "push origin ";
		params.append(TCHAR_TO_UTF8(*BranchName));
		std::string fullCommand = myString + " " + params;
	    int result = system(fullCommand.c_str());
		if (result != 0)
		{
		    std::cerr << "git push failed with exit code " << result << std::endl;
			UE_LOG(LogTemp, Error, TEXT("Failed to push changes to repository...try manually "));
			return FReply::Handled();
		}

		const FText NotificationText = FText::Format(
			LOCTEXT("SourceControlMenu_Success", "Remote Repository Updated"),
			FText::FromName(FName("Save"))
		);
		FNotificationInfo Info(NotificationText);
		Info.ExpireDuration = 8.0f;
		FSlateNotificationManager::Get().AddNotification(Info);

		/*	TSharedRef<FCheckIn, ESPMode::ThreadSafe> CheckInOperation = ISourceControlOperation::Create<FCheckIn>();
			bool bCheckinSuccess = SourceControlModule.GetProvider().Execute(CheckInOperation, FilesToSubmit) == ECommandResult::Succeeded;
			if (bCheckinSuccess)
			{
				UE_LOG(LogTemp, Error, TEXT("Checkin sucess "));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Checkin...fail..try manually "));
			}*/

		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Source Control is disabled"));
		return FReply::Handled();
	}

	//ToDo : Need to explore these interfaces for source control operations
	

	//TSharedRef<FUpdateStatus, ESPMode::ThreadSafe> SaveOperation = ISourceControlOperation::Create<FUpdateStatus>();
	//SaveOperation->SetUpdateModifiedState(true);
	//ECommandResult::Type Result = GitSourceControl.GetProvider().Execute(SaveOperation,
	//	TArray<FString>(), EConcurrency::Synchronous);

	//if (Result == ECommandResult::Succeeded)
	//{
	//	const FText NotificationText = FText::Format(
	//		LOCTEXT("SourceControlMenu_Success", "Operation Sucess: {1} operation "),
	//		FText::FromName(FName("Save"))
	//	);
	//	FNotificationInfo Info(NotificationText);
	//	Info.ExpireDuration = 8.0f;
	//	FSlateNotificationManager::Get().AddNotification(Info);
	//	UE_LOG(LogTemp, Display, TEXT("%s"), *NotificationText.ToString());
	//}
	//else
	//{
	//	// Report failure with a notification 
	//	const FText NotificationText = FText::Format(
	//		LOCTEXT("SourceControlMenu_Failure", "Save Error: {0} operation failed!"),
	//		FText::FromName(FName("Save"))
	//	);

	//	FNotificationInfo Info(NotificationText);
	//	Info.ExpireDuration = 8.0f;
	//	FSlateNotificationManager::Get().AddNotification(Info);
	//	UE_LOG(LogTemp, Error, TEXT("%s"), *NotificationText.ToString());

	//}

	return FReply::Handled();
}

FReply FRealBandBackUpUIManagerImpl::JoinSession()
{
	if (pConcertSyncClient && ServerId.IsValid())
	{
		pConcertSyncClient->GetConcertClient()->GetServerSessions(ServerId).
			Next([iConcertSyncClient = pConcertSyncClient, iServerId = ServerId](FConcertAdmin_GetAllSessionsResponse Response)
				{
					if (Response.ResponseCode == EConcertResponseCode::Success)
					{
						for (const FConcertSessionInfo& SessionInfo : Response.LiveSessions)
						{
							if (iConcertSyncClient->GetConcertClient()->GetSessionConnectionStatus() ==
								EConcertConnectionStatus::Disconnected)
							{
								//	iConcertSyncClient->GetConcertClient()->StartAutoConnect();
								iConcertSyncClient->GetConcertClient()->JoinSession(iServerId, SessionInfo.SessionId);
								UE_LOG(LogTemp, Log, TEXT("=======JOINED  SESSION =============="));
								
							}
						}
					}

					if (Response.ResponseCode == EConcertResponseCode::Failed)
					{
						UE_LOG(LogTemp, Error, TEXT("=======  Failed to Join SESSION =============="));
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to Join SESSION"));
					}

					if (Response.ResponseCode == EConcertResponseCode::Pending)
					{
						UE_LOG(LogTemp, Error, TEXT("=======  Pending to Join SESSION =============="));
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to Join SESSION:Pending"));
					}
				});

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("=======  Failed to Join SESSION....No Server Sessions =============="));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to Join Session ...Try Restarting the Server "));

	}
	return FReply::Handled();
}


bool FRealBandBackUpUIManagerImpl::InitSourceVersionControl()
{

	FSourceControlModule& SourceControlModule = FSourceControlModule::Get();
	if (!SourceControlModule.IsEnabled())
	{
		FGitSourceControlModule& GitSourceControl = FModuleManager::LoadModuleChecked<FGitSourceControlModule>("GitSourceControl");
		GitSourceControl.GetProvider().Init();
		SourceControlModule.ShowLoginDialog(FSourceControlLoginClosed(), ELoginWindowMode::Modal, EOnLoginWindowStartup::PreserveProvider);
		FString texStatus = SourceControlModule.GetProvider().GetStatusText().ToString();

	}


	// check Git availability
	FGitSourceControlModule& GitSourceControl = FModuleManager::LoadModuleChecked<FGitSourceControlModule>("GitSourceControl");
	GitSourceControl.GetProvider().Init();
	
	if (!GitSourceControl.GetProvider().IsGitAvailable())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to find Git Bash installation"));
		return false;
	}

	remoteUrl = GitSourceControl.GetProvider().GetRemoteUrl();
	if (remoteUrl.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Remote Git Url not configured"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Remote Git Url not configured"));
		return false;
	}

	

	//GitSourceControlUtils::RunUpdateStatus()
	//if (GitSourceControlUtils::FindGitBinaryPath().IsEmpty())
	//{
	//	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to find Git Bash installation"));
	//	return false;
	//}

	FString PythonExecutable;// = TEXT("python");
	GetPythonPath(PythonExecutable);

	if (PythonExecutable.IsEmpty())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to find Python Installation"));
		return false;
	}

	
	//FString ScriptPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()).Append("BackUp/automate_git.py");
	FString ScriptPath = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir()).Append("BackUp/automate_git.py");
	
	bool isValidPath = FPaths::FileExists(ScriptPath);
	if (isValidPath)
	{
		bool isValid = true;
		int debug = 1;
		UE_LOG(LogTemp, Display, TEXT("Valid Path"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Path %s"), *ScriptPath);
		return false;
	}

	FString projectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	if (projectPath.EndsWith(TEXT("/")))
	{
		projectPath.RemoveAt(projectPath.Len() - 1, 1);
	}
	

	FString ScriptArguments = projectPath;
	FString Command = FString::Printf(TEXT("\"%s\" \"%s\" \"%s\""), *PythonExecutable, *ScriptPath, *ScriptArguments);
	UE_LOG(LogTemp, Display, TEXT("Python Command  %s"), *Command);
	int Output=-1;
	FString Error;
	FString stdOut;
	int32 ReturnCode = -1;
	FPlatformProcess::ExecProcess(*Command, *ScriptArguments, &ReturnCode, &stdOut, &Error);

	if (ReturnCode != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to update the repository %s"), *projectPath);
		UE_LOG(LogTemp, Error, TEXT("Output %s Error %s ReturnCode %d"), *stdOut,*Error,ReturnCode);
		return false;
	}


	//ISourceControlModule& SourceControlModule = FModuleManager::LoadModuleChecked<ISourceControlModule>("SourceControl");
	//ProjectDirHandle.BindLambda([this]() { 
	//	//return FString(TEXT("path/to/source/control/project/root"));
	//	FString projectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	//	if (projectPath.EndsWith(TEXT("/")))
	//	{
	//		projectPath.RemoveAt(projectPath.Len() - 1, 1);
	//	}
	//	return projectPath;
	//	//return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	//	});
	//
	//SourceControlModule.RegisterSourceControlProjectDirDelegate(ProjectDirHandle);
	//FString srcPrjDir = SourceControlModule.GetSourceControlProjectDir();
	//UE_LOG(LogTemp, Display, TEXT("Project Dir registered with Source Control %s"), *srcPrjDir);
	//SourceControlModule.SetProvider("Git");
	//SourceControlModule.SetUseGlobalSettings(false);
	//
	//ISourceControlProvider & srcctrlProvider = SourceControlModule.GetProvider();
	////SourceControlModule.GetProvider().Login()

	//FString providerName = srcctrlProvider.GetName().GetPlainNameString();

	//bool bIsEnabled = SourceControlModule.IsEnabled();

	return true;
}
	
void FRealBandBackUpUIManagerImpl::GetPythonPath(FString& oPythonPath)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Python\\PythonCore"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		UE_LOG(LogTemp, Display, TEXT("Python installation found"));
		TCHAR Version[MAX_PATH];
		DWORD VersionLength = ARRAYSIZE(Version);
		DWORD Index = 0;
		DWORD ErrorCode;

		while ((ErrorCode = RegEnumKeyExW(hKey, Index++, Version, &VersionLength, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
		{
			HKEY hSubKey;
			FString SubKeyName = FString::Printf(TEXT("SOFTWARE\\Python\\PythonCore\\%s\\InstallPath"), Version);
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, *SubKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				TCHAR Path[MAX_PATH];
				DWORD PathLength = ARRAYSIZE(Path);
				if (RegQueryValueEx(hSubKey, TEXT("ExecutablePath"), NULL, NULL, (LPBYTE)Path, &PathLength) == ERROR_SUCCESS)
				{
					oPythonPath = FString(Path);
					break;
				}
				RegCloseKey(hSubKey);
			}
			VersionLength = ARRAYSIZE(Version);
		}
		RegCloseKey(hKey);
	}
	if (oPythonPath.IsEmpty())
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Python\\PythonCore"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			UE_LOG(LogTemp, Display, TEXT("Python installation found"));
			TCHAR Version[MAX_PATH];
			DWORD VersionLength = ARRAYSIZE(Version);
			DWORD Index = 0;
			DWORD ErrorCode;
			while ((ErrorCode = RegEnumKeyExW(hKey, Index++, Version, &VersionLength, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
			{
				HKEY hSubKey;
				FString SubKeyName = FString::Printf(TEXT("SOFTWARE\\Python\\PythonCore\\%s\\InstallPath"), Version);
				if (RegOpenKeyEx(HKEY_CURRENT_USER, *SubKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					TCHAR Path[MAX_PATH];
					DWORD PathLength = ARRAYSIZE(Path);
					if (RegQueryValueEx(hSubKey, TEXT("ExecutablePath"), NULL, NULL, (LPBYTE)Path, &PathLength) == ERROR_SUCCESS)
					{
						oPythonPath = FString(Path);
						break;
					}
					RegCloseKey(hSubKey);
				}
				VersionLength = ARRAYSIZE(Version);
			}
			RegCloseKey(hKey);

		}
	}

	if (oPythonPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find Python executable path"));
	}
}

bool FRealBandBackUpUIManagerImpl::GetGitBinaryPath(FString& oGitBinaryPath)
{
	bool bRet = false;
	HKEY key;
	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\GitForWindows"), 0, KEY_READ, &key);

	if (result == ERROR_SUCCESS) {
		char path[MAX_PATH] = { 0 };
		DWORD pathSize = sizeof(path);

		TCHAR Path[MAX_PATH];
		DWORD PathLength = ARRAYSIZE(Path);
		if (RegQueryValueEx(key, TEXT("InstallPath"), NULL, NULL, (LPBYTE)Path, &PathLength) == ERROR_SUCCESS)
		{
			oGitBinaryPath = FString(Path);
			UE_LOG(LogTemp, Display, TEXT("Git install path: %s"), *oGitBinaryPath);
			bRet = true;
		}
		else
		{
			bRet = false;
		}

		RegCloseKey(key);
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

#undef LOCTEXT_NAMESPACE