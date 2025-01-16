// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/SNOnlineSystemCommonV1.h"

#include "Online/SNSessionSearchResult.h"
#if COMMONUSER_OSSV1
#include "SNDef.h"
#include "Utility/SNUtility.h"
#include "Online/SNOnlineSystem.h"

#include "OnlineSessionSettings.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

#include "Online/SNOnlineCommonSetting.h"
#include "Online/OnlineSessionNames.h"
#endif
//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//! @param OnlineSystem オーナーとなるOnlineSystemクラス
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::Initialize(USNOnlineSystem* OnlineSystem){
	OwnerOnlineSystem = OnlineSystem;
}

//----------------------------------------------------------------------//
//
//! @brief ログイン処理
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::Login(){
#if COMMONUSER_OSSV1
	// オンラインシステムのインターフェイスを取得
	IOnlineSubsystem* OnlineSubsystem(Online::GetSubsystem(GetWorld()));
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineIdentityPtr Identity(OnlineSubsystem->GetIdentityInterface());
	// OnlineIdentityが有効なものかチェック
	if(Identity.IsValid()){
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		
		if(PlayerController != nullptr){
			// ローカルプレイヤーを取得
			ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
			// コントローラIDを取得
			int Id = LocalPlayer->GetControllerId();
			
			ELoginStatus::Type Status = Identity->GetLoginStatus(Id);
			
			SNPLUGIN_LOG(TEXT("Login Status : %s"), ELoginStatus::ToString(Status));
		}
	}
#endif
}

//----------------------------------------------------------------------//
//
//! @brief ログイン済みかどうかチェック
//
//! @retval true  ログイン済み
//! @retval false 未ログイン
//
//----------------------------------------------------------------------//
bool USNOnlineSystemCommonV1::IsLoggedIn() const {
#if COMMONUSER_OSSV1
	// オンラインシステムのインターフェイスを取得
	IOnlineSubsystem* OnlineSubsystem(Online::GetSubsystem(GetWorld()));
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineIdentityPtr Identity(OnlineSubsystem->GetIdentityInterface());
	// OnlineIdentityが有効なものかチェック
	if(Identity.IsValid()){
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		
		if(PlayerController != nullptr){
			// ローカルプレイヤーを取得
			ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
			// コントローラIDを取得
			int Id = LocalPlayer->GetControllerId();
			
			return (Identity->GetLoginStatus(Id) == ELoginStatus::LoggedIn) ? true : false;
		}
	}
#endif
	return false;
}

//----------------------------------------------------------------------//
//
//! @brief ホストとしてセッションを生成
//
//! @param SessionRequest セッションリクエスト
//! @param Name           セッション名
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNOnlineSystemCommonV1::HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name){
#if COMMONUSER_OSSV1
	
	bool Result = false;
	// オンラインシステムのインターフェイスを取得
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	// セッションインターフェイスを取得
	IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
	
	if(Sessions.IsValid()){
		
		Sessions->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &USNOnlineSystemCommonV1::OnCreateSessionComplete));
		
		TSharedPtr<class FOnlineSessionSettings> SessionSettings(MakeShareable(new FOnlineSessionSettings()));
		
		SNPLUGIN_ASSERT(SessionSettings != nullptr, TEXT("Failed to allocate Session Settings."));
		// 生成するセッションの設定
		SessionSettings->NumPublicConnections			= SessionRequest->MaxPlayerNum;
		SessionSettings->NumPrivateConnections			= 0;
		SessionSettings->bIsLANMatch					= (SessionRequest->OnlineMode == ESessionOnlineMode::LAN) ? true : false;
		SessionSettings->bShouldAdvertise				= SessionRequest->bShouldIdAdvertise;
		SessionSettings->bAllowJoinInProgress			= SessionRequest->bAllowJoinInProgress;
		SessionSettings->bAllowInvites					= SessionRequest->bAllowInvites;
		SessionSettings->bUsesPresence					= SessionRequest->bUsesPresence;
		SessionSettings->bAllowJoinViaPresence			= SessionRequest->bAllowJoinViaPresense;
		SessionSettings->bUseLobbiesIfAvailable			= SessionRequest->bUseLobbiesIfAvailable;
		SessionSettings->bUseLobbiesVoiceChatIfAvailable= SessionRequest->bUseLobbiesVoiceChatIfAvailable;
		
		SessionSettings->Set(SEARCH_KEYWORDS, Name.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		// プレイヤーコントローラがない場合はアサート
		SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
		
		TSharedPtr<const FUniqueNetId> UniqueNetIdPtr(SNUtility::GetUniqueNetId(PlayerController));
		// ネットワームIDが有効かチェック
		if(UniqueNetIdPtr.IsValid()){
			
			bool bResult = Sessions->CreateSession(*UniqueNetIdPtr, Name, *SessionSettings);
			
			if(bResult == true){
				
				SNPLUGIN_LOG(TEXT("Success to Create Session."))
				
				return true;
				
			} else {
				SNPLUGIN_ERROR(TEXT("CreateSession: Fail"));
			}
		} else {
			SNPLUGIN_ERROR(TEXT("CreateSession : Unique Net Id is none."))
		}
	}
#endif
	return false;
}

//----------------------------------------------------------------------//
//
//! @brief セッションを検索
//
//! @param Sessions       セッションインターフェイス
//! @param SearchSettings 検索設定
//
//----------------------------------------------------------------------//
bool USNOnlineSystemCommonV1::FindSession(){
#if COMMONUSER_OSSV1
	
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
	
	if(Sessions.IsValid()){
		
		Sessions->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &USNOnlineSystemCommonV1::OnFindSessionsComplete));
		
		SearchSettings = MakeShareable(new FOnlineSessionSearch());
		
		SearchSettings->SearchResults.Empty();
		// クエリの設定
		SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		
		TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSettings.ToSharedRef();
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		
		SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
		
		TSharedPtr<const FUniqueNetId> UniqueNetIdPtr(SNUtility::GetUniqueNetId(PlayerController));
		
		if(UniqueNetIdPtr.IsValid()){
			// セッションの検索を開始
			bool bResult = Sessions->FindSessions(*UniqueNetIdPtr, SearchSettingsRef);
			
			if(bResult == true){
				SNPLUGIN_LOG(TEXT("Success to FindSession."))
			} else {
				SNPLUGIN_ERROR(TEXT("FindSession: Fail"));
			}
			
			return bResult;
			
		} else {
			SNPLUGIN_ERROR(TEXT("FindSession : Unique Net Id is none."))
		}
	}
#endif
	return false;
}

//----------------------------------------------------------------------//
//
//! @brief セッションに参加
//
//! @param SearchResult 参加するセッションの情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::JoinSession(const USNSessionSearchResult* SearchResult){
#if COMMONUSER_OSSV1
	
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineSessionPtr Sessions(OnlineSubsystem->GetSessionInterface());
	
	if(Sessions.IsValid()){
		
		Sessions->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &USNOnlineSystemCommonV1::OnJoinSessionComplete));
		
		if((SearchResult != nullptr) && (SearchResult->IsValid())){
			// プレイヤーコントローラを取得
			APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
			
			SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
			
			TSharedPtr<const FUniqueNetId> UniqueNetIdPtr(SNUtility::GetUniqueNetId(PlayerController));
			
			if(UniqueNetIdPtr.IsValid()){
			
				FName JoinSessionName(NAME_None);
				// 検索結果にキーワードが含まれているかチェック
				if(SearchResult->Get().Session.SessionSettings.Settings.Contains(SEARCH_KEYWORDS) == true){
					// オンライン設定を取得
					const FOnlineSessionSetting& Setting(SearchResult->Get().Session.SessionSettings.Settings[SEARCH_KEYWORDS]);
					// データからセッション名を取得
					JoinSessionName = *Setting.Data.ToString();
				}

				OwnerOnlineSystem->SetConnectSession(SearchResult);
				// セッションに参加
				Sessions->JoinSession(*UniqueNetIdPtr, JoinSessionName, SearchResult->Get());
			} else {
				SNPLUGIN_ERROR(TEXT("JointSession : Unique Net Id is none."));
			}
		} else {
			SNPLUGIN_ERROR(TEXT("Invalid session."));
		}
	}
#endif
}

//----------------------------------------------------------------------//
//
//! @brief セッションを終了
//
//! @param SessionName セッション名
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::KillSession(const FName& SessionName){
#if COMMONUSER_OSSV1
	
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
	
	if(Sessions.IsValid()){
		// セッションを終了
		Sessions->DestroySession(SessionName);
	}
	
	ConnectURL = TEXT("");
#endif
}

FUniqueNetIdRepl USNOnlineSystemCommonV1::GetLocalUserNetId(APlayerController* PlayerController) const {
#if COMMONUSER_OSSV1
	
	IOnlineIdentityPtr Identity(Online::GetIdentityInterface(GetWorld()));
	
	if(Identity.IsValid()){

		APawn* Pawn(PlayerController->GetPawn());

		if(Pawn != nullptr)
		{
			FPlatformUserId UserId(Pawn->GetPlatformUserId());

			return FUniqueNetIdRepl(Identity->GetUniquePlayerId(UserId));
		} 
	}
#endif
	return FUniqueNetIdRepl();
}

//----------------------------------------------------------------------//
//
//! @brief FindSessionの検索結果を取得
//
//! @retval FindSessionの検索結果
//
//----------------------------------------------------------------------//
TArray<TObjectPtr<USNSessionSearchResult>> USNOnlineSystemCommonV1::GetSessionSearchResultList() const {
#if COMMONUSER_OSSV1
	return SessionSerchResultList;
#else
	return TArray<TObjectPtr<USNSessionSearchResult>>();
#endif
}


//----------------------------------------------------------------------//
//
//! @brief セッション生成に成功した際に呼ばれるデリゲート
//
//! @param InSessionName  セッション名
//! @param bWasSuccessful セッション生成に成功したかどうかのフラグ
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful){
#if COMMONUSER_OSSV1
	
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	
	if(OwnerOnlineSystem->OnCompleteHostSession.IsBound()){
		OwnerOnlineSystem->OnCompleteHostSession.Broadcast(InSessionName, bWasSuccessful);
	}
	
	if(bWasSuccessful == true){

		IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
		SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
		IOnlineSessionPtr Sessions(OnlineSubsystem->GetSessionInterface());
	
		if(Sessions.IsValid())
		{
			FOnlineSession* OnlineSession = Sessions->GetNamedSession(InSessionName);

			if(OnlineSession != nullptr)
			{
				FOnlineSessionSearchResult Result;
				
				Result.Session = *OnlineSession;
				// サーチリザルトとして登録
				USNSessionSearchResult* SearchResult = NewObject<USNSessionSearchResult>(this);
				
				*SearchResult = Result;
				// リストに登録
				SessionSerchResultList.Add(SearchResult);

				OwnerOnlineSystem->SetConnectSession(SearchResult);
			}
		}
		
		SNPLUGIN_LOG(TEXT("Create Session Succeed [%s]."), *InSessionName.ToString());
		// マップを移動
		OwnerOnlineSystem->ServerTravel();
	} else {
		SNPLUGIN_ERROR(TEXT("Failed to Session Succeed [%s]."), *InSessionName.ToString());
	}
#endif
}

//----------------------------------------------------------------------//
//
//! @brief セッションの検索が完了した際に呼ばれるデリゲート
//
//! @param bWasSuccessful セッションの検索に成功したかのフラグ
//
//----------------------------------------------------------------------//
void USNOnlineSystemCommonV1::OnFindSessionsComplete(bool bWasSuccessful){
#if COMMONUSER_OSSV1
	
	if(bWasSuccessful == true){
		
		SNPLUGIN_LOG(TEXT("Find Session: Success"));
		// ※エディタ同士の場合検索結果が0になり、セッション検索に失敗したことになります。
		//   スタンドアローン起動なら大丈夫です。
		if(SearchSettings->SearchResults.Num() == 0){
			SNPLUGIN_LOG(TEXT("No session found."));
		} else {
			
			//const TCHAR* SessionId = *SearchSettings->SearchResults[0].GetSessionIdStr();
			// DISPLAY_LOG("Session ID: %s", SessionId);
			// JoinSession(SearchSettings->SearchResults[0]);
			
			for(auto Result:SearchSettings->SearchResults){
				
				USNSessionSearchResult* SearchResult = NewObject<USNSessionSearchResult>(this);
				
				*SearchResult = Result;
				
				SessionSerchResultList.Add(SearchResult);
			}
		}
	} else {
		// セッション検索失敗
		SNPLUGIN_LOG(TEXT("Find Session: Fail"));
	}
	
	if(OwnerOnlineSystem->OnCompleteFindSession.IsBound()){
		OwnerOnlineSystem->OnCompleteFindSession.Broadcast(bWasSuccessful);
	}
#endif
}

void USNOnlineSystemCommonV1::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result){
#if COMMONUSER_OSSV1
	
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	if(OnlineSubsystem != nullptr){
		
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		
		if(Sessions.IsValid()){
			
			if(Result == EOnJoinSessionCompleteResult::Success){
				
				if(Sessions->GetResolvedConnectString(InSessionName, ConnectURL)){
					
					UE_LOG_ONLINE_SESSION(Log, TEXT("Join session: traveling to %s"), *ConnectURL);
					
					APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
					
					SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
					
					PlayerController->ClientTravel(ConnectURL, TRAVEL_Absolute);
				}
			}
		}
	}
	
	if(OwnerOnlineSystem->OnCompleteJoinSession.IsBound()){
		OwnerOnlineSystem->OnCompleteJoinSession.Broadcast(InSessionName, Result == EOnJoinSessionCompleteResult::Success ? true : false);
	}
#endif
}
