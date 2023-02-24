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
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "Blutility/Classes/EditorUtilityObject.h"

#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/ConcertSettings.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/IConcertClient.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClient.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClientModule.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public/IMultiUserClientModule.h"
#include "../../../../Engine/Plugins/Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public/ConcertFrontendStyle.h"
#include "../../../../Engine\Plugins\Developer\Concert\ConcertSync\ConcertSyncCore\Source\ConcertSyncCore\Public\ConcertLocalFileSharingService.h"
#include "../../../../Engine\Plugins\Messaging\UdpMessaging\Source\UdpMessaging\Public\Shared\UdpMessagingSettings.h"
#include "../../../../Engine\Plugins\Messaging\UdpMessaging\Source\UdpMessaging\Private\Transport\UdpMessageTransport.h"
#include "../../../../Engine/Source/Editor/ContentBrowser/Private/SContentBrowser.h"
#include "../../../../Engine/Plugins\Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public\ConcertFrontendUtils.h"
#include "../../../../Engine/Plugins\Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public\Session\Browser\SConcertSessionBrowser.h"

//Git
#include "../../../../Engine/Plugins\Developer\GitSourceControl\Source\GitSourceControl\Private\GitSourceControlModule.h"
#include "../../../../Engine/Plugins\Developer\GitSourceControl\Source\GitSourceControl\Private\GitSourceControlUtils.h"
#include "SourceControl\Public\SourceControlOperations.h"

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



#define LOCTEXT_NAMESPACE "RealBandBackUP"

FRealBandBackUpUIManager::~FRealBandBackUpUIManager()
{
	FConcertFrontendStyle::Shutdown();
}


FRealBandBackUpUIManagerImpl::~FRealBandBackUpUIManagerImpl()
{
	
	pDialogMainWindow.Reset();
	pOverlay.Reset();
	pCanvas.Reset();
	
	FRealBandBackUpUIManager::Instance.Reset();
	FRealBandBackUpUIManager::Instance = nullptr;
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
		bool isSuccess = Instance->InitSourceVersionControl();
		if (!isSuccess)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to Initialize source control. Check the Debug Output"));
			return;
		}
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
		              .Size(FVector2D(170.0f, 20.0f))
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
						  
						]
						  //.Style
		//.Font(FCoreStyle::Get().GetFontStyle(TEXT("PropertyWindow.NormalFont")))
		            //      .TextStyle(FAppStyle::Get(), "NormalText")

		//.TextStyle(FTextBlockStyle)
////			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)

		              ]

			         + SCanvas::Slot()
			          .HAlign(HAlign_Fill)
			          .VAlign(VAlign_Fill)
			          .Size(FVector2D(120.0f, 50.0f))
			          .Position(FVector2D(30.0f, 180.0f))
			          [
				        SNew(SButton)
				       .HAlign(HAlign_Center)
			           .VAlign(VAlign_Center)
			           .Text(FText::FromString("Start Backup"))
////			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)

			           ]
					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(120.0f, 50.0f))
						 .Position(FVector2D(180.0f, 180.0f))
						 [
							 SNew(SButton)
							 .HAlign(HAlign_Center)
						 .VAlign(VAlign_Center)
						 .Text(FText::FromString("JOIN"))
						 .OnClicked(this, &FRealBandBackUpUIManagerImpl::JoinSession)

						 ]

					 + SCanvas::Slot()
						 .HAlign(HAlign_Fill)
						 .VAlign(VAlign_Fill)
						 .Size(FVector2D(70.0f, 40.0f))
						 .Position(FVector2D(450.0f, 68.0f))
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
						 .Position(FVector2D(550.0f, 68.0f))
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
////	else
////	{
////		if (!pDialogMainWindow->IsVisible())
////		{
////			pDialogMainWindow->SetVisibility(EVisibility::All);
////			pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
////			pRealBandImportSettings->SetVisibility(EVisibility::Visible);
////		}
////    }
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
		FConcertFrontendStyle::Initialize();

		IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
		if (MultiUserClientModule.IsConcertServerRunning())
		{
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
				//OnServersAssumedReady();
				// Try to connect to an existing session even if we launched a new server
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
			pURealBandConfig->FindOrLaunchConcertServer();
			pConcertSyncClient = pURealBandConfig->GetBackupClient();
			pConcertSyncClient->GetConcertClient()->OnKnownServersUpdated().AddRaw(
					this, &FRealBandBackUpUIManagerImpl::OnServersAssumedReady);

			IConcertSyncClientModule::Get().OnClientCreated().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleConcertSyncClientCreated);
			for (TSharedRef<IConcertSyncClient> Client : IConcertSyncClientModule::Get().GetClients())
			{
				Client->OnSyncSessionStartup().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleSyncSessionStartup);
					//Client->OnSyncSessionShutdown().AddRaw(this, &FRealBandBackUpUIManagerImpl::HandleSyncSessionShutdown);
			}

		}
		
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
					//
				}
				else
				{
					pConcertSyncClient->GetConcertClient()->CreateSession(ServerId, sessionArgs);
				}
			}
		}
	}
	
}

void FRealBandBackUpUIManagerImpl::HandleSyncSessionStartup(const IConcertSyncClient* SyncClient)
{

	/*FGuid CurrentSessionId = pConcertSyncClient->GetConcertClient()->GetCurrentSession()->GetId();
	pConcertSyncClient->GetConcertClient()->JoinSession(ServerId, CurrentSessionId).
		Next([](EConcertResponseCode Response)
			{
				if (Response == EConcertResponseCode::Success)
				{
					int debug = 1;
				}

			});*/
	//pConcertSyncClient->GetConcertClient()->GetCurrentSession()->Connect();

	//    pConcertSyncClient->GetConcertClient()->GetServerSessions(ServerId)
	//	.Next([iSyncClient = pConcertSyncClient, iServerId = ServerId](FConcertAdmin_GetAllSessionsResponse Response)
	//		{
	//			FGuid DefaultSessionId;
	//			if (Response.ResponseCode == EConcertResponseCode::Success)
	//			{
	//				// Find our default session IDs
	//				for (const FConcertSessionInfo& SessionInfo : Response.LiveSessions)
	//				{
	//					//if (SessionInfo.SessionName == LocalSettings->DefaultSessionName)
	//					if (SessionInfo.SessionName == "RealBandMultiUser")
	//					{
	//						DefaultSessionId = SessionInfo.SessionId;
	//						if (DefaultSessionId.IsValid() && iSyncClient->GetConcertClient()->GetSessionConnectionStatus() == 
	//							                                     EConcertConnectionStatus::Disconnected)
	//						{

	//							iSyncClient->GetConcertClient()->JoinSession(iServerId, DefaultSessionId);
	//						}
	//						iSyncClient->GetConcertClient()->GetCurrentSession()->Connect();
	//						break;
	//					}
	//				}
	//			}
	//		});

	
	/*FGuid DefaultSessionId = pConcertSyncClient->GetConcertClient()->GetCurrentSession()->GetId();
	if (DefaultSessionId.IsValid())
	{
		
		pConcertSyncClient->GetConcertClient()->JoinSession(ServerId, DefaultSessionId);
	}*/
	int test = 0;
}

void FRealBandBackUpUIManagerImpl::HandleConcertSyncClientCreated(TSharedRef<IConcertSyncClient> Client)
{
	int test = 0;
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

	TSharedRef<FSync, ESPMode::ThreadSafe> SyncOperation = ISourceControlOperation::Create<FSync>();
	ECommandResult::Type Result = GitSourceControl.GetProvider().Execute(SyncOperation,
		                          TArray<FString>(), EConcurrency::Synchronous);
	
	if (Result == ECommandResult::Succeeded)
	{
		const FText NotificationText = FText::Format(
			LOCTEXT("SourceControlMenu_Success", "Operation Sucess: {1} operation "),
			FText::FromName(FName("Sync"))
		);
		FNotificationInfo Info(NotificationText);
		Info.ExpireDuration = 8.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		UE_LOG(LogTemp, Display, TEXT("%s"), *NotificationText.ToString());
	}
	else
	{
		// Report failure with a notification 
		const FText NotificationText = FText::Format(
			LOCTEXT("SourceControlMenu_Failure", "Error: {0} operation failed!"),
			FText::FromName(FName("Sync"))
		);
		
		FNotificationInfo Info(NotificationText);
		Info.ExpireDuration = 8.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		UE_LOG(LogTemp, Error, TEXT("%s"), *NotificationText.ToString());
		
	}

	return FReply::Handled();
}


FReply FRealBandBackUpUIManagerImpl::Save()
{
	TArray<TSharedPtr<FConcertSessionClientInfo>> Clients;
	FConcertSessionClientInfo objClientInfo;
	objClientInfo.ClientEndpointId = pConcertSyncClient->GetConcertClient()->GetCurrentSession()->GetSessionServerEndpointId();
	objClientInfo.ClientInfo = pConcertSyncClient->GetConcertClient()->GetClientInfo();

	TSharedPtr<FConcertSessionClientInfo> ClientSharedObj = MakeShared<FConcertSessionClientInfo>(objClientInfo);
	Clients.Push(ClientSharedObj);

	TArray<TSharedPtr<FConcertSessionClientInfo>> LatestClientPtrs = Clients;

	ConcertFrontendUtils::SyncArraysByPredicate(Clients, MoveTemp(LatestClientPtrs), [](const TSharedPtr<FConcertSessionClientInfo>& ClientToFind)
		{
			return [ClientToFind](const TSharedPtr<FConcertSessionClientInfo>& PotentialClient)
			{
				return PotentialClient->ClientEndpointId == ClientToFind->ClientEndpointId;
			};
		});

	return FReply::Handled();
}

FReply FRealBandBackUpUIManagerImpl::JoinSession()
{
	
	if (pConcertSyncClient && ServerId.IsValid())
	{
		FGuid CurrentSessionId = pConcertSyncClient->GetConcertClient()->GetCurrentSession()->GetId();
		pConcertSyncClient->GetConcertClient()->StartAutoConnect();
		if (CurrentSessionId.IsValid())
		{
			//	TSharedPtr < SConcertSessionBrowser> ConcertSessionBrowser = nullptr;
			//	FGlobalTabmanager::Get()->TryInvokeTab(FTabId(FName("ConcertBrowser")),true);
			if (TSharedPtr<SDockTab> NewTab = FGlobalTabmanager::Get()->GetActiveTab())
			{
				ConcertSessionBrowser = StaticCastSharedRef<SConcertSessionBrowser>(NewTab->GetContent());
				ConcertSessionBrowser->SetCanTick(true);
			}
			TSharedPtr<SDockTab> ActiveTab = FGlobalTabmanager::Get()->GetActiveTab();
			if (ActiveTab.IsValid())
			{
				FString CurrentTab = FGlobalTabmanager::Get()->GetActiveTab()->GetLayoutIdentifier().ToString();
				//	TSharedRef<SConcertBrowser> ContentBrowser = StaticCastSharedRef<SContentBrowser>(ActiveTab->GetContent());
				//	FContentBrowserSelection  selectedItem;
			//		selectedItem.SelectedFolders.Add(CurrentSessionId.ToString());

				//	ContentBrowser->SyncTo(selectedItem);

					// Do something with the content browser here
			}


			//	pConcertSyncClient->GetConcertClient()->OnSessionConnectionChanged().AddRaw(this, &FRealBandBackUpUIManagerImpl::OnSessionConnectionChanged);



			pConcertSyncClient->GetConcertClient()->JoinSession(ServerId, CurrentSessionId).Next([](EConcertResponseCode Response)
				{
					if (Response == EConcertResponseCode::Success)
					{
						UE_LOG(LogTemp, Log, TEXT("=======Join session SUCCESS=============="));
						int debug = 1;
					}

					if (Response == EConcertResponseCode::Failed)
					{
						UE_LOG(LogTemp, Log, TEXT("=======Join session FAILED=============="));
						int debug = 1;

					}

					if (Response == EConcertResponseCode::TimedOut)
					{
						int debug = 1;
					}

					if (Response == EConcertResponseCode::UnknownRequest)
					{
						int debug = 1;
					}

					if (Response == EConcertResponseCode::InvalidRequest)
					{
						int debug = 1;
					}

					int m = 1;
					int n = 2;
				});
			//	pDialogMainWindow->SetVisibility(EVisibility::Collapsed);
			//	pDialogMainWindow->Minimize();
			IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
			//	MultiUserClientModule.OpenBrowser();

			TSharedPtr<SWidget> topWindow = FSlateApplication::Get().GetActiveTopLevelWindow();


			TSharedPtr<SDockTab> bActiveTab = FGlobalTabmanager::Get()->GetActiveTab();
			FText pText = FGlobalTabmanager::Get()->GetApplicationTitle();
			FName tabLevel = bActiveTab->GetTag();

			//	ConcertSessionBrowser = static_cast<SConcertSessionBrowser*>(bActiveTab->GetContent());
		//		ConcertSessionBrowser = StaticCastSharedRef<SConcertSessionBrowser>(bActiveTab->GetContent());


				/*SConcertSessionBrowser *ConcertSessionBrowser = Cast<SConcertSessionBrowser>(topWindow.Get());

				if (ConcertSessionBrowser)
				{
					TArray<TSharedPtr<FConcertSessionTreeItem>> listSessions = ConcertSessionBrowser->GetSelectedItems();
					ConcertSessionBrowser->RefreshSessionList();
				}*/


		}

	}
	//TSharedPtr<SWindow> ContainingWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
	
	return FReply::Handled();
}

void FRealBandBackUpUIManagerImpl::OnSessionConnectionChanged(IConcertClientSession& Session, EConcertConnectionStatus Status)
{
	int debug = 1;
}


bool FRealBandBackUpUIManagerImpl::InitSourceVersionControl()
{
	// check Git availability
	FGitSourceControlModule& GitSourceControl = FModuleManager::LoadModuleChecked<FGitSourceControlModule>("GitSourceControl");
	GitSourceControl.GetProvider().Init();
	if (!GitSourceControl.GetProvider().IsGitAvailable())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Failed to find Git Bash installation"));
		return false;
	}

	FString remoteUrl = GitSourceControl.GetProvider().GetRemoteUrl();
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

	FString ScriptPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()).Append("BackUp/automate_git.py");
	
	bool isValidPath = FPaths::FileExists(ScriptPath);
	if (isValidPath)
	{
		bool isValid = true;
		int debug = 1;
		UE_LOG(LogTemp, Display, TEXT("Valid Path"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InValid Path"));
		return false;
	}

	FString projectPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	if (projectPath.EndsWith(TEXT("/")))
	{
		projectPath.RemoveAt(projectPath.Len() - 1, 1);
	}
	FString ScriptArguments = projectPath;
	FString Command = FString::Printf(TEXT("\"%s\" \"%s\" \"%s\""), *PythonExecutable, *ScriptPath, *ScriptArguments);
	int Output=-1;
	FString Error;
	FString stdOut;
	int32 ReturnCode = -1;
	FPlatformProcess::ExecProcess(*Command, *ScriptArguments, &Output, &stdOut, &Error);

	if (Output != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to update the repository %s"), *projectPath);
		UE_LOG(LogTemp, Error, TEXT("Error Reported %s"), *stdOut);
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

#undef LOCTEXT_NAMESPACE