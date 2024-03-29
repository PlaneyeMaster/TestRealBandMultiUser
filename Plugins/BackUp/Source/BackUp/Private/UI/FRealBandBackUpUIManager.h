//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Docking/SDockTab.h"

#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"

#include "../../../../Engine\Plugins\Developer\Concert\ConcertMain\Source\Concert\Public\ConcertMessages.h"
#include "SourceControl/Private/SourceControlModule.h"

//Level Instance
#include "LevelInstance/LevelInstanceInterface.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "LevelInstance/LevelInstanceEditorPivot.h"


class IConcertSyncClient;
class URealBandConfig;
class SConcertSessionBrowser;
class IConcertClientSession;
class IConcertSessionBrowserController;
class FStructOnScope;
struct FConcertClientInfo;
struct FConcertSyncActivity;



//struct EConcertConnectionStatus;

DECLARE_LOG_CATEGORY_EXTERN(LogManager, Log, All);

DECLARE_MULTICAST_DELEGATE(FLevelChangeEvent);




class FRealBandBackUpUIManagerImpl : public TSharedFromThis<FRealBandBackUpUIManagerImpl>
{

private:
	TSharedPtr<SWindow> pDialogMainWindow;
	TSharedPtr<SOverlay> pOverlay;
	TSharedPtr<SCanvas> pCanvas;
	TSharedPtr<SButton> pJoinBtn;
	TSharedPtr<SButton> pLaunchServerBtn;
	//void SetupMenuItem();
	
	TSharedPtr<IConcertSyncClient> pConcertSyncClient;
	TSharedPtr<IConcertSessionBrowserController> SessionController;
	URealBandConfig* pURealBandConfig;
	FGuid ServerId;
	TSharedPtr < SConcertSessionBrowser> ConcertSessionBrowser;
	FSourceControlProjectDirDelegate ProjectDirHandle;
	void GetPythonPath(FString& oPythonPath);
	bool GetGitBinaryPath(FString& oGitBinaryPath);
	bool IsLevelValid(const FString& iLevelPath);
	FString remoteUrl;
	bool isHostMachine;
	//FLevelChangeEvent OnLevelChange;
	struct FConcertPersistCommand
	{
		TArray<FName> PackagesToPersist;
		TArray<FString> FilesToPersist;
		FText ChangelistDescription;
		bool bShouldSubmit;
		bool bShouldMakeFilesWritable;
	};
	FDelegateHandle OnLevelsChangedDelegateHandle;
	FDelegateHandle OnLevelsSelectionChangedDelegateHandle;
	FDelegateHandle FLevelInstanceChanged;
	FDelegateHandle				 ClientChangeDelegate;
	FDelegateHandle OnAssetSavedHandle;
	FDelegateHandle OnAssetCreatedHandle;
	bool isVersionCtrlSet;

public:

	virtual ~FRealBandBackUpUIManagerImpl();
	void Initialize();
	bool isVersionCtrlConfigured() { return isVersionCtrlSet; }
	static void SetupMenuItem();
	static void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	void CreateWidgetWindow();
	bool InitSourceVersionControl();
	void InitMultiUserEditorControls();
	void OnServersAssumedReady();
	void HandleSyncSessionStartup(const IConcertSyncClient* SyncClient);
	void HandleSyncSessionShutdown(const IConcertSyncClient* SyncClient);
	void HandleConcertSyncClientCreated(TSharedRef<IConcertSyncClient> Client);
	void OnSessionConnectionChanged(IConcertClientSession& Session, EConcertConnectionStatus Status);
	/** Invoked when the session connection state is changed. */
	void HandleSessionConnectionChanged(IConcertClientSession& InSession, EConcertConnectionStatus ConnectionStatus);
	void HandleActivityAddedOrUpdated(const FConcertClientInfo& InClientInfo, const FConcertSyncActivity& InActivity,
		                              const FStructOnScope& InActivitySummary);
	void OnRecoverySessionShutdown(TSharedRef<IConcertClientSession> InSession);
	void onTick(IConcertClientSession&, float);
	bool SubmitChangelist(const FConcertPersistCommand& PersistCommand, FText& OperationMessage);
	FReply JoinSession();
	FReply LaunchServer();
	void OnLevelChangedCallback(bool bStatus);
	UFUNCTION()
	void OnLevelShown() ;
	//void OnSessionConnectionChanged(IConcertClientSession& InSession, EConcertConnectionStatus ConnectionStatus);
	FReply Save();
	FReply Sync();
	void AssetSaved(const FAssetData& iAsset);
	void AssetUpdated(const FAssetData& AssetData);
    static FLevelChangeEvent OnLevelChange;
	//static FLevelInstanceChanged onInstanceLevelChanged;

	DECLARE_EVENT_OneParam(ULevelInstanceSubsystem, FLevelInstanceChanged, FName);
	void OnPreviewControlRigUpdated() { 
		;
	}

	void UpdateLevel(ULevel*, ULevel*, UWorld*);
	
};


class FRealBandBackUpUIManager
{
public:
	static void Initialize();
	void Setup();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	//static bool DisplayStartWindow();
	static TSharedPtr<FRealBandBackUpUIManagerImpl> Instance;
	~FRealBandBackUpUIManager();
};
