// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/RealBandConfig.h"
#include "CoreGlobals.h"
#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"


//Editor
#include "LevelEditor.h"
#include "UdpMessagingSettings.h"
//#include "MultiUserServerModule.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "Blutility/Classes/EditorUtilityObject.h"

#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/ConcertSettings.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/IConcertClient.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertMain/Source/Concert/Public/IConcertServer.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClient.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public/IConcertSyncClientModule.h"
#include "../../../../Engine/Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public/IMultiUserClientModule.h"
#include "../../../../Engine/Plugins/Developer\Concert\ConcertApp\MultiUserServer\Source\MultiUserServer\Public\IMultiUserServerModule.h"
#include "../../../../Engine/Plugins/Developer\Concert\ConcertUI\ConcertSharedSlate\Source\ConcertSharedSlate\Public/ConcertFrontendStyle.h"
#include "IConcertModule.h"

#include "ConcertSyncSettings.h"

#include "ConcertTransactionEvents.h"
#include "IConcertClientTransactionBridge.h"
#include "AssetToolsModule.h"
#include "AssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"


namespace MultiUsrConfigUtl
{
	FString AppendRandomNumbersToString(const FString InString, uint8 NumberToAppend = 6)
	{
		FString RandomizedString = "_";
		for (uint8 RandomIteration = 0; RandomIteration < NumberToAppend; RandomIteration++)
		{
			RandomizedString += FString::FromInt(FMath::RandRange(0, 9));
		}
		return InString + RandomizedString;
	}
}


URealBandConfig::URealBandConfig()
{
	
}

URealBandConfig::~URealBandConfig()

{
	/*if (BackupClient->GetConcertClient()->IsStarted())
	{
		BackupClient->Shutdown();
	}*/
}

void URealBandConfig::Save(const FAssetData& iAsset)
{

}


//void URealBandConfig::SetConfig(TSharedPtr<IConcertSyncClient>  BackupClient )
//{
//	IMultiUserClientModule& MultiUserClientModule = FModuleManager::LoadModuleChecked<IMultiUserClientModule>("MultiUserClient");
//
//	
//	if (MultiUserClientModule.IsAvailable())
//	{
//		BackupClient = IConcertSyncClientModule::Get().CreateClient("MultiUser");
//		if (!MultiUserClientModule.GetClient()->GetConcertClient()->GetConfiguration()->bInstallEditorToolbarButton)
//		{
//			 // The button in the Level editor toolbar.
//
//
//			const FString BackupServerName = GetBackupServerName();
//			const FString BackupSessionName = MakeSessionName();
//
//			UConcertClientConfig* userSettings = NewObject<UConcertClientConfig>();;;
//			userSettings->bInstallEditorToolbarButton = true;
//			userSettings->ClientSettings.DisplayName = FPlatformProcess::UserName();
//			userSettings->ClientSettings.AvatarColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.0f);
//		///	userSettings->ClientSettings.ServerPort = 6666;
//			userSettings->DefaultServerURL = BackupServerName;
//			userSettings->DefaultSessionName = BackupSessionName;
//			userSettings->DefaultSaveSessionAs = BackupSessionName;
//
//			userSettings->ClientSettings.DiscoveryTimeoutSeconds = 10;
//			BackupClient->GetConcertClient()->Configure(userSettings);
//
//			// Configure UDP Messaging
//			FConfigFile* EngineConfig = GConfig ? GConfig->FindConfigFileWithBaseName(FName(TEXT("Engine"))) : nullptr;
//			if (EngineConfig)
//			{
//				TArray<FString> Settings;
//				FString Setting;
//				// Unicast endpoint setting
//				EngineConfig->GetString(TEXT("/Script/UdpMessaging.UdpMessagingSettings"), TEXT("UnicastEndpoint"), Setting);
//			}
//
//
//			BackupClient->GetConcertClient()->Startup();
//			BackupClient->GetConcertClient()->StartDiscovery();
//
//			if (BackupClient->GetConcertClient()->IsStarted())
//			{
//				bool isStarted = true;
//			}
//
//			
//
//			MultiUserClientModule.GetClient()->Startup(userSettings,EConcertSyncSessionFlags::Default_MultiUserSession);
//
//			MultiUserClientModule.GetClient()->GetConcertClient()->StartDiscovery();
//			if (MultiUserClientModule.GetClient()->GetConcertClient()->IsStarted())
//			{
//				bool isStarted = true;
//			}
//			//MultiUserClientModule.GetClient()->GetConcertClient()->SendDiscoverServersEvent();
//
//		}
//
//		if (!MultiUserClientModule.IsConcertServerRunning())
//		{
//			MultiUserClientModule.OpenBrowser();
//
//			FServerLaunchOverrides Overrides;
//			Overrides.ServerName = FString("ALPHA");
//			TOptional<FProcHandle> ServerHandle = MultiUserClientModule.LaunchConcertServer(Overrides);
//		
//			
//			//if (ServerHandle.IsSet() && ServerHandle.GetValue().IsValid())
//			//{
//			//	ServerTrackingData.MultiUserServerHandle = ServerHandle.GetValue();
//			//}
//			
//			
//			MultiUserClientModule.GetClient()->GetConcertClient()->OnKnownServersUpdated();
//			/*	IMultiUserServerModule& MultiUserServerModule = FModuleManager::LoadModuleChecked<IMultiUserServerModule>("MultiUserServer");
//				if (MultiUserServerModule.IsAvailable())
//				{
//					bool iStarted = true;
//				}*/
//				//FConcertServerInfo
//				//MultiUserClientModule.GetClient()->GetConcertClient()->
//			
//			TArray<FConcertServerInfo> ServerList = MultiUserClientModule.GetClient()->GetConcertClient()->GetKnownServers();
//			FGuid ServerId;
//			for (FConcertServerInfo serverInst : ServerList)
//			{
//				if (!serverInst.ServerName.Compare("ALPHA"))
//				{
//					ServerId = serverInst.AdminEndpointId;
//				}
//			}
//
//			FConcertCreateSessionArgs CreateArgs;
//			IConcertClientRef Client = MultiUserClientModule.GetClient()->GetConcertClient();
//			const UConcertClientConfig* ClientConfig = Client->GetConfiguration();
//			CreateArgs.SessionName = FString("XOM");
//			CreateArgs.ArchiveNameOverride = FString("XOM");
//			//MultiUserClientModule.GetClient()->GetConcertClient()->CreateSession(ServerId, CreateArgs);
//		}
//
//	    
//
//		
//	}
//}


void URealBandConfig::ConfigureUDP()
{
	FString ConcertArguments;
	ConcertArguments = GetConcertArguments(GetConcertServerName(), GetConcertSessionName());

}

FString URealBandConfig::GetConcertArguments(const FString& ServerName, const FString& SessionName)
{
	const UConcertClientConfig* ConcertClientConfig = GetDefault<UConcertClientConfig>();
	ensureAlwaysMsgf(ConcertClientConfig, TEXT("%hs: Unable to launch nDisplay because there is no UConcertClientConfig object."), __FUNCTION__);
	FString ReturnValue =
		FString::Printf(TEXT("-CONCERTISHEADLESS -CONCERTRETRYAUTOCONNECTONERROR -CONCERTAUTOCONNECT -CONCERTSERVER=\"%s\" -CONCERTSESSION=\"%s\""),
			*ServerName, *SessionName);
	return ReturnValue;
}

FString URealBandConfig::GetBackupServerName()
{
	// Requirements: To avoid collision, the name must be unique on the local machine (multiple instances) and across the local network (multiple users).
	static FGuid BackupSessionGuid = FGuid::NewGuid();
	return BackupSessionGuid.ToString();
}

FString URealBandConfig::MakeSessionName()
{
	// Convention: The session name starts with the server name (uniqueness), followed by a zero-based unique sequence number (idendify the latest session reliably), the the session creation time and the project name.
	static TAtomic<int32> SessionNum(0);
	return FString("Session1");
	//return FString::Printf(TEXT("%s_%d_%s_%s"), GetBackupServerName(), SessionNum++, *FDateTime::UtcNow().ToString(), FApp::GetProjectName());
}


void URealBandConfig::LaunchConcertServer(const FString & iServerName)
{
	IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
	if(!MultiUserClientModule.IsConcertServerRunning())
	{
		//MultiUserClientModule.OpenBrowser();
		FServerLaunchOverrides Overrides;
		Overrides.ServerName = iServerName;
		ConcertServerRequestStatus = EConcertServerRequestStatus::LaunchRequested;
		TOptional<FProcHandle> ServerHandle = MultiUserClientModule.LaunchConcertServer(Overrides);
		if (ServerHandle.IsSet() && ServerHandle.GetValue().IsValid())
		{
			ServerTrackingData.MultiUserServerHandle = ServerHandle.GetValue();
		}
	}
}


void URealBandConfig::RegisterSettings()
{
	
}


TSharedPtr<IConcertSyncClient> URealBandConfig::CreateConcertClient()
{
	IConcertModule& ConcertModule = FModuleManager::LoadModuleChecked<IConcertModule>("Concert");
	if (ConcertModule.IsAvailable())
	{
		FConcertFrontendStyle::Initialize();
		static FGuid ServernGuid = FGuid::NewGuid();
		const FString BackupServerName = FApp::GetProjectName();
		static TAtomic<int32> SessionNum(0);
		const FString BackupSessionName = FString::Printf(TEXT("%s_%d_%s_%s"), *BackupServerName, SessionNum++, *FDateTime::UtcNow().ToString(), FApp::GetProjectName());

		// Create and populate the client config object
		UConcertClientConfig* ClientConfig = NewObject<UConcertClientConfig>();
		ClientConfig->bIsHeadless = true;
		ClientConfig->bInstallEditorToolbarButton = true;
		ClientConfig->bAutoConnect = false;
		ClientConfig->DefaultServerURL = BackupServerName;
		ClientConfig->DefaultSessionName = BackupSessionName;
		ClientConfig->DefaultSaveSessionAs = BackupSessionName;
		ClientConfig->ClientSettings.AvatarColor = FLinearColor(1.0, 0.0, 0.0);
		ClientConfig->ClientSettings.DisplayName = FPlatformProcess::UserName();
		ClientConfig->EndpointSettings.RemoteEndpointTimeoutSeconds = 0; // Ensure the endpoints never time out (and are kept alive automatically by Concert).
		ClientConfig->ClientSettings.ClientAuthenticationKey = BackupServerName; // The server adds its own server name to the list of authorized client keys, use that key to authorize this client on the server.

		// Create and start the client.
		//BackupClient = IConcertSyncClientModule::Get().CreateClient("MultiUser");
		IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
		BackupClient = MultiUserClientModule.GetClient();
		BackupClient->GetConcertClient()->Configure(ClientConfig);
		BackupClient->Startup(ClientConfig, EConcertSyncSessionFlags::Default_MultiUserSession);
		BackupClient->GetConcertClient()->StartDiscovery();
		

		
	}
	
	return BackupClient;
}

bool URealBandConfig::FindOrLaunchConcertServer()
{
	IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
	if (MultiUserClientModule.IsConcertServerRunning())
	{
		FDelegateHandle OnPostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &URealBandConfig::WorldLoaded);
		return true;
	}

	if (!MultiUserClientModule.IsConcertServerRunning())
	{
	    IConcertModule& ConcertModule = FModuleManager::LoadModuleChecked<IConcertModule>("Concert");
	    if (ConcertModule.IsAvailable())
	    {
		    FConcertFrontendStyle::Initialize();
		    static FGuid ServernGuid = FGuid::NewGuid();
		    //const FString BackupServerName = ServernGuid.ToString() + FApp::GetProjectName();
			const FString BackupServerName = FApp::GetProjectName();
		    static TAtomic<int32> SessionNum(0);
		    const FString BackupSessionName =  FString::Printf(TEXT("%s_%d_%s_%s"), *BackupServerName, SessionNum++, *FDateTime::UtcNow().ToString(), FApp::GetProjectName());
		
		    // Create and populate the client config object
		    UConcertClientConfig* ClientConfig = NewObject<UConcertClientConfig>();
		    ClientConfig->bIsHeadless = true;
		    ClientConfig->bInstallEditorToolbarButton = true;
		    ClientConfig->bAutoConnect = true;
		    ClientConfig->DefaultServerURL = BackupServerName;
		    ClientConfig->DefaultSessionName = BackupSessionName;
		    ClientConfig->DefaultSaveSessionAs = BackupSessionName;
		    ClientConfig->ClientSettings.AvatarColor = FLinearColor(1.0, 0.0, 0.0);
		    ClientConfig->ClientSettings.DisplayName = FPlatformProcess::UserName();
		    ClientConfig->EndpointSettings.RemoteEndpointTimeoutSeconds = 0; // Ensure the endpoints never time out (and are kept alive automatically by Concert).
		    ClientConfig->ClientSettings.ClientAuthenticationKey = BackupServerName; // The server adds its own server name to the list of authorized client keys, use that key to authorize this client on the server.

		    // Create and start the client.
		    //BackupClient = IConcertSyncClientModule::Get().CreateClient("MultiUser");
			BackupClient = MultiUserClientModule.GetClient();

		    BackupClient->Startup(ClientConfig, EConcertSyncSessionFlags::Default_MultiUserSession);
		//BackupClient->GetConcertClient()->StartDiscovery();
		
		//	MultiUserClientModule.OpenBrowser();
			FServerLaunchOverrides Overrides;
			Overrides.ServerName = BackupServerName;
			ConcertServerRequestStatus = EConcertServerRequestStatus::LaunchRequested;
			TOptional<FProcHandle> ServerHandle = MultiUserClientModule.LaunchConcertServer(Overrides);
			if (ServerHandle.IsSet() && ServerHandle.GetValue().IsValid())
			{
				ServerTrackingData.MultiUserServerHandle = ServerHandle.GetValue();
			}
		}

		//LaunchConcertServer(BackupServerName);
		BackupClient->GetConcertClient()->StartDiscovery();
		
		UWorld* World = GEngine->GetWorldContexts()[0].World();
		ULevel *pCurrentLevel = World->GetCurrentLevel();
		ULevel *pLevel =   World->GetLevel(0);
	//	FWorldDelegates::OnCurrentLevelChanged.AddSP(this, &URealBandConfig::UpdateLevel, World->GetCurrentLevel(), World->GetCurrentLevel(), World);
		FWorldDelegates::OnCurrentLevelChanged.AddUObject(this, &URealBandConfig::UpdateLevel);
		
		FWorldDelegates::LevelAddedToWorld.AddUObject(this, &URealBandConfig::LevelAddedToWorld);

		FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &URealBandConfig::LevelAddedToWorld);

		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &URealBandConfig::MapLoaded);

		FDelegateHandle OnPostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &URealBandConfig::WorldLoaded);

		for (ULevel* iLevel : World->GetLevels())
		{
			int debug = 1;
			


			//StreamingLevel->OnLevelShown.Add(FRealBandBackUpUIManagerImpl::OnLevelShown);
		//	StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &FRealBandBackUpUIManagerImpl::OnLevelShown);
		//	FWorldDelegates::LevelAddedToWorld.AddLambda([this, StreamingLevel, World](const ULevelStreaming& world)
		//		{
				//	StreamingLevel->BroadcastLevelLoadedStatus(World, StreamingLevel->GetPackage()->GetFName(), true);
		//		});



			//(this, &FRealBandBackUpUIManagerImpl::OnLevelChangedCallback, true);
		//	StreamingLevel->OnLevelLoaded.Add(FRealBandBackUpUIManagerImpl::OnLevelChangedCallback);
		}

	}
	return false;
	// Ensure we have the client, otherwise we can't do anything
//	TSharedPtr<IConcertSyncClient> ConcertSyncClient = IConcertSyncClientModule::Get().GetClient(TEXT("MultiUser"));
//	if (!ConcertSyncClient)
//	{
//		//UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("%hs: The ConcertSyncClient could not be found. Please check the output log for errors and try again."), __FUNCTION__);
//		return;
//	}
//	//UConcertClientConfig 
//	//const UConcertClientConfig* CurrentConfig = ConcertSyncClient->GetConcertClient()->GetConfiguration();
//
//
//	//ConcertSyncClient->Startup(CurrentConfig, EConcertSyncSessionFlags::Default_MultiUserSession);
//	const IConcertClientRef ConcertClient = ConcertSyncClient->GetConcertClient();
//	const TSharedPtr<IConcertClientSession> ConcertClientSession = ConcertClient->GetCurrentSession();
//	bool bIsConnectedToSession = ConcertClientSession.IsValid();
//	ConcertClient->OnKnownServersUpdated().RemoveAll(this);
//
//	// Shutdown existing server no matter what because we need to hook into OnServersAssumedReady
//	IMultiUserClientModule& MultiUserClientModule = IMultiUserClientModule::Get();
//
//	if (FPlatformProcess::IsProcRunning(ServerTrackingData.MultiUserServerHandle))
//	{
//		// Try to reuse last server
//		TArray<FConcertServerInfo> Servers = ConcertClient->GetKnownServers();
//		FConcertServerInfo* Server = Algo::FindByPredicate(Servers, [Name = GetConcertServerName()](const FConcertServerInfo& Server)
//			{
//				return Server.ServerName == Name;
//			});
//		if (Server)
//		{
//			ConcertServerRequestStatus = EConcertServerRequestStatus::ReuseExisting;
//			OnServersAssumedReady();
//		}
//		else
//		{
//			// We are waiting for servers to register with Concert.
//		/*	ConcertClient->OnKnownServersUpdated().AddRaw(
//				this, &URealBandConfig::OnServersAssumedReady
//			);*/
//		}
//	}
//	else if (MultiUserClientModule.IsConcertServerRunning() || bIsConnectedToSession)
//	{
//		ConcertServerRequestStatus = EConcertServerRequestStatus::ReuseExisting;
//		OnServersAssumedReady();
//	}
//	else
//	{
//		// We are waiting for servers to register with Concert.
////		LaunchConcertServer(BackupServerName);
//	}

	//ConcertClient->Startup();
//	ConcertClient->StartDiscovery();
}


void URealBandConfig::UpdateLevel(ULevel* InNewLevel, ULevel* InOldLevel, UWorld* InWorld)
{
	int test = 1;
}

void URealBandConfig::LevelAddedToWorld(ULevel* iLevel, UWorld* iWorld)
{
	int test = 1;
}

void URealBandConfig::WorldLoaded(UWorld* iWorld, const UWorld::InitializationValues IVS)
{
	int test = 1;
	if (iWorld)
	{
		//iWorld->BroadcastLevelsChanged();
	}
}

void URealBandConfig::MapLoaded(UWorld* iWorld)
{

}

TSharedPtr<IConcertSyncClient> URealBandConfig::GetBackupClient()
{
	return BackupClient;
}

void URealBandConfig::JoinSession()
{

}

void URealBandConfig::OnServersAssumedReady()
{
	if (ConcertServerRequestStatus == EConcertServerRequestStatus::ShutdownRequested)
	{
		// If this method was called when trying to shut down the previous server
		// then loop back so we can return after the new server is launched
		ConcertServerRequestStatus = EConcertServerRequestStatus::None;
		FindOrLaunchConcertServer();
	}
	else if (ConcertServerRequestStatus == EConcertServerRequestStatus::LaunchRequested ||
		ConcertServerRequestStatus == EConcertServerRequestStatus::ReuseExisting)
	{
		FindAppropriateServer();
	}
	else
	{
	//	UE_LOG(LogDisplayClusterLaunchEditor, Warning, TEXT("%hs: OnServersAssumedReady was called when ConcertServerRequestStatus was None."), __FUNCTION__);
	}
}


void URealBandConfig::ConnectToSession()
{
	// Session Management
	// First check to see if we're in a session already
	if (const TSharedPtr<IConcertSyncClient> ConcertSyncClient = IConcertSyncClientModule::Get().GetClient(TEXT("MultiUser")))
	{
		const IConcertClientRef ConcertClient = ConcertSyncClient->GetConcertClient();
		if (const TSharedPtr<IConcertClientSession> ConcertClientSession = ConcertClient->GetCurrentSession())
		{
			// If we're already connected, go straight into launch
			//LaunchDisplayClusterProcess();
			return;
		}

		const UConcertClientConfig* CurrentConfig = ConcertClient->GetConfiguration();
		UConcertClientConfig* AutoConnectConfig = DuplicateObject(CurrentConfig, GetTransientPackage(), CurrentConfig->GetFName());
		AutoConnectConfig->bAutoConnect = false;
		AutoConnectConfig->bRetryAutoConnectOnError = true;
		AutoConnectConfig->DefaultServerURL = GetConcertServerName();
		AutoConnectConfig->DefaultSessionName = GetConcertSessionName();

		ConcertClient->Configure(AutoConnectConfig);
		if (!ConcertClient->IsConfigured())
		{
		//	UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("%hs: The ConcertSyncClient is not configured. Please check the output log for errors and try again."), __FUNCTION__);
			return;
		}

		// Initiate the auto connect to the named server and session.
		if (ConcertClient->CanAutoConnect())
		{
			ConcertClient->StartAutoConnect();
			//LaunchDisplayClusterProcess();
		}
		else
		{
			ConcertClient->Configure(CurrentConfig);
			//UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("Unable to start Multi-user auto connect routines."));
		}
	}
}

const FString& URealBandConfig::GetConcertServerName()
{
	// If the Cached name is changed after this point it will need to be cleared in order to run this code again 
	if (ServerTrackingData.GeneratedMultiUserServerName.IsEmpty())
	{
		ServerTrackingData.GeneratedMultiUserServerName = MultiUsrConfigUtl::AppendRandomNumbersToString("nDisplayLaunchServer");
	}
	return ServerTrackingData.GeneratedMultiUserServerName;
}

const FString& URealBandConfig::GetConcertSessionName()
{
	// If the Cached name is changed after this point it will need to be cleared in order to run this code again 
	if (CachedConcertSessionName.IsEmpty())
	{
		/*const UDisplayClusterLaunchEditorProjectSettings* Settings = GetDefault<UDisplayClusterLaunchEditorProjectSettings>();
		if (!Settings->ExplicitSessionName.IsEmpty())
		{
			CachedConcertSessionName = Settings->ExplicitSessionName;
		}
		else*/
		{
			CachedConcertSessionName = MultiUsrConfigUtl::AppendRandomNumbersToString("nDisplayLaunchSession");
		}
	}
	return CachedConcertSessionName;
}

bool  URealBandConfig::ConfigureUDPMessaging(FString& ConcatenatedArguments)
{
	FConfigFile* EngineConfig = GConfig ? GConfig->FindConfigFileWithBaseName(FName(TEXT("Engine"))) : nullptr;
	if (EngineConfig)
	{
		TArray<FString> Settings;
		FString Setting;
		// Unicast endpoint setting
		EngineConfig->GetString(TEXT("/Script/UdpMessaging.UdpMessagingSettings"), TEXT("UnicastEndpoint"), Setting);
		// if the unicast endpoint port is bound, concatenate it
		if (Setting != "0.0.0.0:0" && !Setting.IsEmpty())
		{
			ConcatenatedArguments += TEXT(" -UDPMESSAGING_TRANSPORT_UNICAST=") + Setting;
		}
		// Multicast endpoint setting
		EngineConfig->GetString(TEXT("/Script/UdpMessaging.UdpMessagingSettings"), TEXT("MulticastEndpoint"), Setting);
		ConcatenatedArguments += TEXT(" -UDPMESSAGING_TRANSPORT_MULTICAST=") + Setting;
		// Static endpoints setting
		Settings.Empty(1);
		EngineConfig->GetArray(TEXT("/Script/UdpMessaging.UdpMessagingSettings"), TEXT("StaticEndpoints"), Settings);
		if (Settings.Num() > 0)
		{
			ConcatenatedArguments += TEXT(" -UDPMESSAGING_TRANSPORT_STATIC=");
			ConcatenatedArguments += Settings[0];
			for (int32 i = 1; i < Settings.Num(); ++i)
			{
				ConcatenatedArguments += ',';
				ConcatenatedArguments += Settings[i];
			}
		}
		return true;
	}
	return false;
}

void  URealBandConfig::FindAppropriateServer()
{
	ConcertServerRequestStatus = EConcertServerRequestStatus::None;
	const TSharedPtr<IConcertSyncClient> ConcertSyncClient = IConcertSyncClientModule::Get().GetClient(TEXT("MultiUser"));
	if (!ConcertSyncClient)
	{
		return;
		//UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("%hs: The ConcertSyncClient could not be found. Please check the output log for errors and try again."), __FUNCTION__);
	}


	const IConcertClientRef ConcertClient = ConcertSyncClient->GetConcertClient();
	// Try to connect to an existing session even if we launched a new server
	TArray<FConcertServerInfo> Servers = ConcertClient->GetKnownServers();
	FConcertServerInfo* Server = nullptr;
	if (const TSharedPtr<IConcertClientSession> ConcertClientSession = ConcertClient->GetCurrentSession())
	{
		const FConcertSessionInfo& SessionInfo = ConcertClientSession->GetSessionInfo();
		Server = Algo::FindByPredicate(Servers, [&SessionInfo](const FConcertServerInfo& Server)
			{
				return Server.InstanceInfo.InstanceId == SessionInfo.ServerInstanceId;
			});
		// The list of known servers must contain the connected session.
		if (!Server)
		{
			//UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("%hs: The list of known servers does not contain the connected session. Please check the output log for errors and try again."), __FUNCTION__);
			return;
		}
		CachedConcertSessionName = ConcertClientSession->GetName();
	}
	else
	{
		// If no session, we need to try to find a server with a name matching the cached name
		Server = Algo::FindByPredicate(Servers, [Name = GetConcertServerName()](const FConcertServerInfo& Server)
			{
				return Server.ServerName == Name;
			});
		if (!Server && Servers.Num() > 0)
		{
			// Pick a server to use.
			Server = &Servers[0];
		}
	}

	if (Server)
	{
		ServerTrackingData.MultiUserServerInfo = *Server;
		ServerTrackingData.GeneratedMultiUserServerName = Server->ServerName;
		ConnectToSession();
	}
	else
	{
		//UE_LOG(LogDisplayClusterLaunchEditor, Error, TEXT("%hs: Servers exist but a matching server was not found. Try connecting to a server and session manually."), __FUNCTION__);
		return;
	}
}





