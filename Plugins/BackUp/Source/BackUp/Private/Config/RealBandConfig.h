// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "CoreGlobals.h"
#include "CoreMinimal.h"
#include "ConcertMessageData.h"

#include "RealBandConfig.generated.h"

class IConcertSyncClient;
/**
 * 
 */
UCLASS(Config = Editor)
class URealBandConfig : public UObject
{
	GENERATED_BODY()

	FString	GetConcertArguments(const FString& ServerName, const FString& SessionName);
	const FString& GetConcertServerName();
	void OnServersAssumedReady();
	void ConnectToSession();
	const FString& GetConcertSessionName();
	FString CachedConcertSessionName;
	TSharedPtr<IConcertSyncClient> BackupClient;
	void RegisterSettings();
public:
	virtual ~URealBandConfig();
	enum class EConcertServerRequestStatus
	{
		None,
		ShutdownRequested,
		LaunchRequested,
		ReuseExisting
	};

	EConcertServerRequestStatus ConcertServerRequestStatus = EConcertServerRequestStatus::None;

	struct FLaunchMultiUserServerTrackingData
	{
		FString GeneratedMultiUserServerName;
		FConcertServerInfo MultiUserServerInfo;
		FProcHandle MultiUserServerHandle;
	};


	FLaunchMultiUserServerTrackingData ServerTrackingData;
	bool FindOrLaunchConcertServer();
	bool ConfigureUDPMessaging(FString& ConcatenatedArguments);
	FString GetBackupServerName();
	FString MakeSessionName();

	URealBandConfig();
//	void SetConfig(TSharedPtr<IConcertSyncClient> );
	void ConfigureUDP();
	void LaunchConcertServer(const FString & );
	void FindAppropriateServer();
	void JoinSession();
	TSharedPtr<IConcertSyncClient> GetBackupClient();
	TSharedPtr<IConcertSyncClient> CreateConcertClient();
};
