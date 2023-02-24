//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Docking/SDockTab.h"

#include "../../../../Engine\Plugins\Developer\Concert\ConcertMain\Source\Concert\Public\ConcertMessages.h"
#include "SourceControl/Private/SourceControlModule.h"


class IConcertSyncClient;
class URealBandConfig;
class SConcertSessionBrowser;
class IConcertClientSession;
//struct EConcertConnectionStatus;

DECLARE_LOG_CATEGORY_EXTERN(LogManager, Log, All);



class FRealBandBackUpUIManagerImpl : public TSharedFromThis<FRealBandBackUpUIManagerImpl>
{

private:

	TSharedPtr<SWindow> pDialogMainWindow;
	TSharedPtr<SOverlay> pOverlay;
	TSharedPtr<SCanvas> pCanvas;
	void SetupMenuItem();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	TSharedPtr<IConcertSyncClient> pConcertSyncClient;
	URealBandConfig* pURealBandConfig;
	FGuid ServerId;
	TSharedPtr < SConcertSessionBrowser> ConcertSessionBrowser;
	FSourceControlProjectDirDelegate ProjectDirHandle;
	void GetPythonPath(FString& oPythonPath);
public:

	virtual ~FRealBandBackUpUIManagerImpl();
	void Initialize();
	void CreateWidgetWindow();
	bool InitSourceVersionControl();
	void InitMultiUserEditorControls();
	void OnServersAssumedReady();
	void HandleSyncSessionStartup(const IConcertSyncClient* SyncClient);
	void HandleConcertSyncClientCreated(TSharedRef<IConcertSyncClient> Client);
	void OnSessionConnectionChanged(IConcertClientSession& Session, EConcertConnectionStatus Status);
	FReply JoinSession();
	FReply Save();
	FReply Sync();
};


class FRealBandBackUpUIManager
{
public:
	static void Initialize();
	//static bool DisplayStartWindow();
	static TSharedPtr<FRealBandBackUpUIManagerImpl> Instance;
	~FRealBandBackUpUIManager();
};
