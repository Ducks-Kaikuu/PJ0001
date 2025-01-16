// Fill out your copyright notice in the Description page of Project Settings.
#include "Online/SNOnlineSystemEOSV2.h"

#include "SNDef.h"
#include "Character/SNPlayerController.h"
#include "Utility/SNUtility.h"
#if !COMMONUSER_OSSV1
#include "Online/Auth.h"
#include "Online/Lobbies.h"
#include "Online/Sessions.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/OnlineResult.h"
#include "Online/OnlineServices.h"
#include "Online/OnlineServicesEngineUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Online/SNOnlineCommonSetting.h"
#include "Online/SNOnlineSystem.h"
#include "Online/SNSessionSearchResult.h"
#endif


//----------------------------------------------------------------------//
// 後々Platform指定に変更するかも…。今はPlatformにするとサービスの取得に失敗するので、とりあえず。
//----------------------------------------------------------------------//
#define SN_ONLINE_SERVICE_TYPE (UE::Online::EOnlineServices::Default)

#if !COMMONUSER_OSSV1
namespace{
	const FName SETTING_ONLINESUBSYSTEM_VERSION(TEXT("OSSv2"));
}
#endif

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//! @param OnlineSystem オーナーとなるOnlineSystemクラス
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::Initialize(USNOnlineSystem* OnlineSystem){
#if !COMMONUSER_OSSV1
	
	OwnerOnlineSystem = OnlineSystem;
	
	UGameInstance* GameInstance(SNUtility::GetGameInstance<UGameInstance>());
	
	if(GameInstance != nullptr){
		// 専用サーバーを持っているかチェック
		bIsDedicatedServer = GameInstance->IsDedicatedServerInstance();
	}

	BindDelegator();
#endif
}

#if !COMMONUSER_OSSV1
void USNOnlineSystemEOSV2::BindDelegator()
{
	UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
	
	UE::Online::ILobbiesPtr Lobbies(OnlineServices->GetLobbiesInterface());
	// サーバーと一部のクライアントしかデリゲートは呼ばれません。
	JoinedDelegateHandle = Lobbies->OnLobbyMemberJoined().Add(this, &USNOnlineSystemEOSV2::HandleLobbyJoined);
	// サーバーと一部のクライアントしかデリゲートは呼ばれません。
	MemberLeftDelegateHandle = Lobbies->OnLobbyMemberLeft().Add(this, &USNOnlineSystemEOSV2::HandleMemberLeftLobby);
}
#endif

//----------------------------------------------------------------------//
//
//! @brief ログイン処理
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::Login(){
#if !COMMONUSER_OSSV1
	
	APawn* Pawn(SNUtility::GetCurrentPlayer<APawn>());
	
	FPlatformUserId UserId(Pawn->GetPlatformUserId());
	
	if(IsRealPlatformUser(UserId) == false){
		
		SNPLUGIN_ERROR(TEXT("Invalid Platform User."));
		
		return;
	}
	// すでにログイン済みかチェック
	if(IsLoggedIn() == true){
		
		SNPLUGIN_LOG(TEXT("Already Logged in."));
		
		return;
	}
	// エディタの場合はDeveloperモードでログイン
	// DevAuthToolを使用する必要あり。
	UE::Online::FAuthLogin::Params LoginParameters;
	
	LoginParameters.PlatformUserId	= UserId;

#if WITH_EDITOR
#if 1
	LoginParameters.CredentialsType	= UE::Online::LoginCredentialsType::Developer;
	LoginParameters.CredentialsId	= TEXT("localhost:7777");
	LoginParameters.CredentialsToken.Emplace<FString>(OwnerOnlineSystem->GetOnlineToken().ToString());
#else
	LoginParameters.CredentialsType	= UE::Online::LoginCredentialsType::AccountPortal;
#endif
#else
	LoginParameters.CredentialsType	= UE::Online::LoginCredentialsType::AccountPortal;
#endif
	UE::Online::IAuthPtr AuthInterface(GetAuth());
	
	UE::Online::TOnlineAsyncOpHandle<UE::Online::FAuthLogin> LoginHandle = AuthInterface->Login(MoveTemp(LoginParameters));
	
	LoginHandle.OnComplete(this, &USNOnlineSystemEOSV2::HandleUserLoginCompleted, UserId);
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
bool USNOnlineSystemEOSV2::IsLoggedIn() const{
#if !COMMONUSER_OSSV1
	
	APawn* Pawn(SNUtility::GetCurrentPlayer<APawn>());
	
	FPlatformUserId UserId(Pawn->GetPlatformUserId());
	
	if(IsRealPlatformUser(UserId) == false){
		return false;
	}
	
	UE::Online::IAuthPtr AuthInterface(GetAuth());
	
	UE::Online::FAuthGetLocalOnlineUserByPlatformUserId::Params AccountParams = {UserId};
	
	UE::Online::TOnlineResult<UE::Online::FAuthGetLocalOnlineUserByPlatformUserId> GetAccountResult(AuthInterface->GetLocalOnlineUserByPlatformUserId(MoveTemp(AccountParams)));
	
	if(GetAccountResult.IsOk()){
		
		TSharedPtr<UE::Online::FAccountInfo> AccountInfo(GetAccountResult.GetOkValue().AccountInfo);
		
		return (AccountInfo->LoginStatus == UE::Online::ELoginStatus::LoggedIn) ? true : false;
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
bool USNOnlineSystemEOSV2::HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name){
#if !COMMONUSER_OSSV1
	
	if(IsLoggedIn() == false){
		
		SNPLUGIN_LOG(TEXT("EOS Not Loggedin."));
		
		return false;
	}
	
	ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("Player Controller is nullptr."));
	
	if(PlayerController->IsLocalController() == false){
		return false;
	}
	
	UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
	
	UE::Online::ILobbiesPtr Lobbies(OnlineServices->GetLobbiesInterface());
	
	SNPLUGIN_ASSERT(Lobbies != nullptr, TEXT("OnlineServices interface is nullptr."));
	
	UE::Online::FCreateLobby::Params Params;
	
	FUniqueNetIdRepl UniqueNetIdRepl(GetLocalUserNetId(PlayerController));
	
	Params.LocalAccountId = UniqueNetIdRepl.GetV2();
	
	Params.LocalName = Name;
	
	Params.SchemaId = UE::Online::FSchemaId(TEXT("GameLobby"));
	
	Params.bPresenceEnabled = true;
	Params.MaxMembers = SessionRequest->MaxPlayerNum;
	Params.JoinPolicy = UE::Online::ELobbyJoinPolicy::PublicAdvertised;
	
	Params.Attributes.Emplace(SETTING_MAPNAME, SessionRequest->MapName);
	Params.Attributes.Emplace(SETTING_MATCHING_TIMEOUT, 120.0f);
	Params.Attributes.Emplace(SETTING_ONLINESUBSYSTEM_VERSION, true);
	
	if(SessionRequest->bUsesPresence){
		Params.Attributes.Emplace(SEARCH_PRESENCE, true);
	}
	// セッション名の受け渡しのために「SETTING_SESSION_TEMPLATE_NAME」を使っていいのか…どうなのか…
	Params.Attributes.Emplace(SETTING_SESSION_TEMPLATE_NAME, Name.ToString());
	
	Params.UserAttributes.Emplace(SETTING_GAMEMODE, FString(TEXT("GameSession")));
	
	UE::Online::TOnlineAsyncOpHandle<UE::Online::FCreateLobby> LobbyHandle = Lobbies->CreateLobby(MoveTemp(Params));
	
	LobbyHandle.OnComplete(this, &USNOnlineSystemEOSV2::HandleUserCreateSessionCompleted, Name);
	
	return true;
#else 
	return false;
#endif
}

//----------------------------------------------------------------------//
//
//! @brief セッションを検索
//
//! @param Sessions       セッションインターフェイス
//! @param SearchSettings 検索設定
//
//----------------------------------------------------------------------//
bool USNOnlineSystemEOSV2::FindSession(){
#if !COMMONUSER_OSSV1
	
	if(IsLoggedIn() == false){
		
		SNPLUGIN_LOG(TEXT("EOS Not Loggedin."));
		
		return false;
	}
	
	ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("Player Controller is nullptr."));
	
	if(PlayerController->IsLocalController() == false){
		
		SNPLUGIN_LOG(TEXT("PlayerController is not LocalController."));
		
		return false;
	}
	// 検索結果を一旦リセット
	SessionSerchResultList.Empty();
	
	UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
	
	UE::Online::ILobbiesPtr Lobbies(OnlineServices->GetLobbiesInterface());
	
	SNPLUGIN_ASSERT(Lobbies != nullptr, TEXT("OnlineServices interface is nullptr."));
	
	UE::Online::FFindLobbies::Params FindLobbyParams;
	
	FUniqueNetIdRepl UniqueNetIdRepl(GetLocalUserNetId(PlayerController));
	
	FindLobbyParams.LocalAccountId = UniqueNetIdRepl.GetV2();
	
	FindLobbyParams.MaxResults = 10;
	
	FindLobbyParams.Filters.Emplace(UE::Online::FFindLobbySearchFilter{ SETTING_ONLINESUBSYSTEM_VERSION, UE::Online::ESchemaAttributeComparisonOp::Equals, true });
	
	FindLobbyParams.Filters.Emplace(UE::Online::FFindLobbySearchFilter{ SEARCH_PRESENCE, UE::Online::ESchemaAttributeComparisonOp::Equals, true });
	
	UE::Online::TOnlineAsyncOpHandle<UE::Online::FFindLobbies> FindHandle(Lobbies->FindLobbies(MoveTemp(FindLobbyParams)));
	
	FindHandle.OnComplete(this, &USNOnlineSystemEOSV2::HandleUserFindSessionComplete);
	
	return true;
#else
	return false;
#endif
}

//----------------------------------------------------------------------//
//
//! @brief セッションに参加
//
//! @param SearchResult 参加するセッションの情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::JoinSession(const USNSessionSearchResult* SearchResult){
#if !COMMONUSER_OSSV1
	
	TSharedPtr<UE::Online::IOnlineServices> OnlineServices(GetOnlineServices());
	
	TSharedPtr<UE::Online::ILobbies> Lobbies(OnlineServices->GetLobbiesInterface());
	
	SNPLUGIN_ASSERT(Lobbies != nullptr, TEXT("OnlineServices interface is nullptr."));
	
	ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("Player Controller is nullptr."));
	
	FUniqueNetIdRepl UniqueNetIdRepl(GetLocalUserNetId(PlayerController));

	UE::Online::FJoinLobby::Params JoinParams;
	
	JoinParams.LocalAccountId	= UniqueNetIdRepl.GetV2();
	JoinParams.LocalName		= SearchResult->GetSessionName();
	JoinParams.LobbyId			= SearchResult->Get()->LobbyId;
	JoinParams.bPresenceEnabled	= true;
	
	UE::Online::TOnlineAsyncOpHandle<UE::Online::FJoinLobby> JoinHandle(Lobbies->JoinLobby(MoveTemp(JoinParams)));
	
	JoinHandle.OnComplete(this, &USNOnlineSystemEOSV2::HandleUserJoinSessionComplete, SearchResult);
	
#endif
}

//----------------------------------------------------------------------//
//
//! @brief セッションを終了
//
//! @param SessionName セッション名
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::KillSession(const FName& SessionName){
#if !COMMONUSER_OSSV1
	
	UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
	
	SNPLUGIN_ASSERT(OnlineServices != nullptr, TEXT("OnlineServices interface is nullptr."));
	
	UE::Online::ILobbiesPtr Lobbies(OnlineServices->GetLobbiesInterface());
	
	SNPLUGIN_ASSERT(Lobbies != nullptr, TEXT("Lobby interface is nullptr."));
	
	ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("Player Controller is nullptr."));
	
	FUniqueNetIdRepl UniqueNetIdRepl(GetLocalUserNetId(PlayerController));
	
	UE::Online::FAccountId LocalUserId = UniqueNetIdRepl.GetV2();
	
	UE::Online::FLobbyId Id = OwnerOnlineSystem->GetConnectSession()->Get()->LobbyId;
	
	if(LocalUserId.IsValid() == false || Id.IsValid() == false){
		return;
	}
	
	UE::Online::TOnlineAsyncOpHandle<UE::Online::FLeaveLobby> LeaveHandle = Lobbies->LeaveLobby({LocalUserId, Id});
	
	LeaveHandle.OnComplete(this, &USNOnlineSystemEOSV2::HandleUserLeaveLobby);
	
#endif
}

FUniqueNetIdRepl USNOnlineSystemEOSV2::GetLocalUserNetId(APlayerController* PlayerController) const
{
#if !COMMONUSER_OSSV1

	FPlatformUserId UserId(PlayerController->GetPlatformUserId());
	
	UE::Online::IAuthPtr AuthInterface(GetAuth());
	
	UE::Online::FAuthGetLocalOnlineUserByPlatformUserId::Params AccountParams = {UserId};
	
	UE::Online::TOnlineResult<UE::Online::FAuthGetLocalOnlineUserByPlatformUserId> GetAccountResult(AuthInterface->GetLocalOnlineUserByPlatformUserId(MoveTemp(AccountParams)));
	
	if(GetAccountResult.IsOk())
	{
		TSharedPtr<UE::Online::FAccountInfo> AccountInfo(GetAccountResult.GetOkValue().AccountInfo);
		
		if(AccountInfo != nullptr){
			return FUniqueNetIdRepl(AccountInfo->AccountId);
		}
	}
#endif
	
	return FUniqueNetIdRepl();
}

ELoginStatusType USNOnlineSystemEOSV2::GetLocalUserLoginStatus(FPlatformUserId PlatformUser){
	
	ELoginStatusType Result = ELoginStatusType::NotLoggedIn;
	
#if !COMMONUSER_OSSV1
	
	if(IsRealPlatformUser(PlatformUser) == false){
		return Result;
	}
	
	if(TSharedPtr<UE::Online::FAccountInfo> AccountInfo = GetOnlineServiceAccountInfo(PlatformUser)){
		Result = AccountInfo->LoginStatus;
	}
#endif
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief FindSessionの検索結果を取得
//
//! @retval FindSessionの検索結果
//
//----------------------------------------------------------------------//
TArray<TObjectPtr<USNSessionSearchResult>> USNOnlineSystemEOSV2::GetSessionSearchResultList() const {
#if !COMMONUSER_OSSV1
	return SessionSerchResultList;
#else
	return TArray<TObjectPtr<USNSessionSearchResult>>();
#endif
}

#if !COMMONUSER_OSSV1
FUniqueNetIdRepl USNOnlineSystemEOSV2::GetLocalUserNetId(FPlatformUserId PlatformUser) const {
	
	if(IsRealPlatformUser(PlatformUser) == false){
		return FUniqueNetIdRepl();
	}
	
	if(TSharedPtr<UE::Online::FAccountInfo> AccountInfo = GetOnlineServiceAccountInfo(PlatformUser)){
		return FUniqueNetIdRepl(AccountInfo->AccountId);
	} else {
		ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());

		if(PlayerController != nullptr)
		{
			return GetLocalUserNetId(PlayerController);	
		}
	}
	
	return FUniqueNetIdRepl();
}

//----------------------------------------------------------------------//
//
//! @brief 指定されたユーザーのアカウント情報を取得
//
//! @param PlatformUser ユーザーID
//
//! @retval アカウント情報
//
//----------------------------------------------------------------------//
TSharedPtr<UE::Online::FAccountInfo> USNOnlineSystemEOSV2::GetOnlineServiceAccountInfo(FPlatformUserId PlatformUser) const {
	
	TSharedPtr<UE::Online::FAccountInfo> AccountInfo;
	// 認証インターフェイスを取得
	UE::Online::IAuthPtr AuthInterface(GetAuth());
	// アカウント取得用のパラメータを設定
	UE::Online::FAuthGetLocalOnlineUserByPlatformUserId::Params GetAccountParams = { PlatformUser };
	// アカウント情報を取得
	UE::Online::TOnlineResult<UE::Online::FAuthGetLocalOnlineUserByPlatformUserId> AccountResult = AuthInterface->GetLocalOnlineUserByPlatformUserId(MoveTemp(GetAccountParams));
	// 取得できたかチェック
	if(AccountResult.IsOk()){
		AccountInfo = AccountResult.GetOkValue().AccountInfo;
	} else {
		// エラーを表示
		SNPLUGIN_ERROR(TEXT("GetOnlineServiceAccountInfo() Failed.[Error] : %s"), *AccountResult.GetErrorValue().GetLogString());
	}
	
	return AccountInfo;
}

//----------------------------------------------------------------------//
//
//! @brief オンラインサービスを取得
//
//! @retval オンラインサービスへのポインタ
//
//! @note GameServicesがWorldに紐づいていて、Worldがnullptrの場合、属性Noneの
//!       GameServiceが作られて中途半端にログインやセッションの生成に成功しますが、
//!       Joinに失敗します。
//!       そのため、WorldはGetPrimaryWorld()で取得し、取得できない場合はアサート
//!       するので、アサートした場合はコール側のタイミングを見直して下さい。
//
//----------------------------------------------------------------------//
UE::Online::IOnlineServicesPtr USNOnlineSystemEOSV2::GetOnlineServices() const {
	// Worldを取得
	UWorld* World(GetPrimaryWorld());
	
	SNPLUGIN_ASSERT(World != nullptr, TEXT("[USNOnlineSystemEOSV2::GetOnlineServices] : World is nullptr."));
	// オンラインサービスを取得
	UE::Online::IOnlineServicesPtr OnlineServices(GetServices(World, SN_ONLINE_SERVICE_TYPE));
	
	SNPLUGIN_ASSERT(OnlineServices != nullptr, TEXT("[USNOnlineSystemEOSV2::GetOnlineServices] : OnlineServices is nullptr."));
	
	return OnlineServices;
}

//----------------------------------------------------------------------//
//
//! @brief 認証インターフェイスを取得
//
//! @retval 認証インターフェイス
//
//----------------------------------------------------------------------//
UE::Online::IAuthPtr USNOnlineSystemEOSV2::GetAuth() const {
	// オンラインサービスを取得
	UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
	// 認証インターフェイスを取得
	UE::Online::IAuthPtr AuthInterface(OnlineServices->GetAuthInterface());
	
	SNPLUGIN_ASSERT(AuthInterface != nullptr, TEXT("Auth Interface is nullptr."));
	
	return AuthInterface;
}

bool USNOnlineSystemEOSV2::IsRealPlatformUser(FPlatformUserId PlatformUser) const {
	// ユーザーIDが有効かチェック
	if(PlatformUser.IsValid() == false){
		
		SNPLUGIN_ERROR(TEXT("Platform User Id is Invalid."));
		
		return false;
	}
	
	if(PlatformUser != IPlatformInputDeviceMapper::Get().GetPrimaryPlatformUser()){
		return false;
	}
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief EOSへのログインが完成した際のデリゲート
//
//! @param Result       リザルト情報
//! @param PlatformUser ログインしたユーザーID
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleUserLoginCompleted(const UE::Online::TOnlineResult<UE::Online::FAuthLogin>& Result, FPlatformUserId PlatformUser){
	
	const bool bWasSuccessful = Result.IsOk();
	
	if(bWasSuccessful == true){
		SNPLUGIN_LOG(TEXT("Player Login Completed - UserIdx:%d Successful:%d NewId:%s"), PlatformUser.GetInternalId(), (int32)Result.IsOk(), *ToLogString(Result.GetOkValue().AccountInfo->AccountId))
	} else {
		SNPLUGIN_ERROR(TEXT("Player Login Failed - UserIdx:%d Successful:%d"), PlatformUser.GetInternalId(), (int32)Result.IsOk());
	}
	
	if(OwnerOnlineSystem->OnCompleteLoggedIn.IsBound()){
		OwnerOnlineSystem->OnCompleteLoggedIn.Broadcast(bWasSuccessful, (bWasSuccessful == true) ? TEXT("") : Result.GetErrorValue().GetLogString());
	}
}

//----------------------------------------------------------------------//
//
//! @brief セッションの生成が終了した際に呼ばれるデリゲート
//
//! @param Result      生成結果
//! @param SessionName 生成したセッション名
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleUserCreateSessionCompleted(const UE::Online::TOnlineResult<UE::Online::FCreateLobby>& Result, FName SessionName){
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	
	const bool bWasSuccessful = Result.IsOk();
	// デリゲートを実行
	if(OwnerOnlineSystem->OnCompleteHostSession.IsBound()){
		OwnerOnlineSystem->OnCompleteHostSession.Broadcast(SessionName, bWasSuccessful);
	}
	
	if(bWasSuccessful == true){
		// サーチリザルトとして登録
		USNSessionSearchResult* SearchResult = NewObject<USNSessionSearchResult>(this);
		// ロビー情報を保存
		*SearchResult = Result.GetOkValue().Lobby;
		// リストに登録
		SessionSerchResultList.Add(SearchResult);
		// 接続したセッション情報を設定
		OwnerOnlineSystem->SetConnectSession(SearchResult);
		// オンラインサービスを取得
		UE::Online::IOnlineServicesPtr OnlineServices(GetOnlineServices());
		// ロビーインターフェイスを取得
		UE::Online::ILobbiesPtr LobbiesInterface(OnlineServices->GetLobbiesInterface());
		
		
		
		SNPLUGIN_LOG(TEXT("Create Session Succeed [%s]."), *SessionName.ToString());
		// マップを移動
		OwnerOnlineSystem->ServerTravel();
	} else {
		SNPLUGIN_ERROR(TEXT("Failed to Session Succeed [%s]."), *SessionName.ToString());
	}
}

//----------------------------------------------------------------------//
//
//! @brief セッションの検索が完了した際のデリゲート
//
//! @param FindResult リザルト情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleUserFindSessionComplete(const UE::Online::TOnlineResult<UE::Online::FFindLobbies>& FindResult){
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	// 結果を取得
	const bool bWasSuccessful = FindResult.IsOk();
	
	if(bWasSuccessful == true){
		// 成功した結果を取得
		const UE::Online::FFindLobbies::Result& FindResults = FindResult.GetOkValue();
		
		SNPLUGIN_LOG(TEXT("Find Session: Success : %d"), FindResults.Lobbies.Num());
		
		for(auto Lobby : FindResults.Lobbies){
			// すでにメンバー数がそろっているロビーは表示しない
			if(Lobby->MaxMembers > Lobby->Members.Num()){
				// オブジェクトを生成
				USNSessionSearchResult* SearchResult = NewObject<USNSessionSearchResult>(this);
				// ロビー情報を設定
				*SearchResult = Lobby;
				// リストに登録
				SessionSerchResultList.Add(SearchResult);
			}
		}
	}
	// デリゲートを実行
	if(OwnerOnlineSystem->OnCompleteFindSession.IsBound()){
		OwnerOnlineSystem->OnCompleteFindSession.Broadcast(bWasSuccessful);
	}
}

//----------------------------------------------------------------------//
//
//! @brief セッションに参加が完了した際のデリゲート
//
//! @param JoinResult   リザルト情報
//! @param SearchResult ジョインしたセッションの情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleUserJoinSessionComplete(const UE::Online::TOnlineResult<UE::Online::FJoinLobby>& JoinResult, const USNSessionSearchResult* SearchResult){
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	// 接続したセッション情報を設定
	OwnerOnlineSystem->SetConnectSession(SearchResult);
	// セッション名を取得
	FName SessionName(SearchResult->GetSessionName());
	// 結果を取得
	const bool bWasSuccessful = JoinResult.IsOk();
	
	if(OwnerOnlineSystem->OnCompleteJoinSession.IsBound()){
		// 参加処理が
		OwnerOnlineSystem->OnCompleteJoinSession.Broadcast(SessionName, bWasSuccessful);
	}
	// 参加に成功しているかチェック
	if(bWasSuccessful == true){
		
		SNPLUGIN_LOG(TEXT("Join Session: Success : %s"), *SessionName.ToString());
		// オンラインサービスを取得
		TSharedPtr<UE::Online::IOnlineServices> OnlineServices(GetOnlineServices());
		// プレイヤーコントローラを取得
		ASNPlayerController* PlayerController(SNUtility::GetPlayerController<ASNPlayerController>());
		
		SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("Player Controller is nullptr."));
		// ユニークIDを取得
		FUniqueNetIdRepl UniqueNetIdRepl(GetLocalUserNetId(PlayerController));
		// ローカルユーザーIDを取得
		UE::Online::FAccountId LocalUserId = UniqueNetIdRepl.GetV2();
		
		if(LocalUserId.IsValid()){
			
			TSharedPtr<UE::Online::ILobbies> Lobbies(OnlineServices->GetLobbiesInterface());
			
			SNPLUGIN_ASSERT(Lobbies != nullptr, TEXT("OnlineServices interface is nullptr."));
			
			UE::Online::TOnlineResult<UE::Online::FGetResolvedConnectString> Result = OnlineServices->GetResolvedConnectString({LocalUserId, JoinResult.GetOkValue().Lobby->LobbyId});
			
			SNPLUGIN_ASSERT(Result.IsOk(), TEXT("Resolved Connect String is Failed."));
			
			ConnectURL = Result.GetOkValue().ResolvedConnectString;
			
			SNPLUGIN_LOG(TEXT("Join session: traveling to %s"), *ConnectURL);
			// サーバーのレベルにジャンプ
			PlayerController->ClientTravel(ConnectURL, TRAVEL_Absolute);
		} else {
			SNPLUGIN_ERROR(TEXT("Join Failed. LocalUserId is Invalidate."));
		}
	}
}

void USNOnlineSystemEOSV2::HandleMemberLeftLobby(const UE::Online::FLobbyMemberLeft& LeftResult)
{
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	
	if (OwnerOnlineSystem->OnMemberLeftLobby.IsBound())
	{
		OwnerOnlineSystem->OnMemberLeftLobby.Broadcast();
	}
}

//----------------------------------------------------------------------//
//
//! @brief ロビーからの離脱が完了した際のデリゲート
//
//! @param Result リザルト情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleUserLeaveLobby(const UE::Online::TOnlineResult<UE::Online::FLeaveLobby>& Result){
	
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));
	// 結果を取得
	const bool bWasSuccessful = Result.IsOk();
	
	if(OwnerOnlineSystem->OnCompleteLeaveSession.IsBound()){
		// ロビーからの離脱を通知
		OwnerOnlineSystem->OnCompleteLeaveSession.Broadcast(bWasSuccessful);
	}
}

//----------------------------------------------------------------------//
//
//! @brief ロビーにメンバーが参加した際のイベント
//
//! @param JoinedMember 参加したメンバーの情報
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV2::HandleLobbyJoined(const UE::Online::FLobbyMemberJoined& JoinedMember){
	// オーナーが存在するかチェック
	SNPLUGIN_ASSERT(OwnerOnlineSystem != nullptr, TEXT("Owner Online System is nullptr."));

	if (OwnerOnlineSystem->OnMemberJoineLotty.IsBound())
	{
		OwnerOnlineSystem->OnMemberJoineLotty.Broadcast();
	}
	
	LobbyMembers.Add(JoinedMember.Member);
}
#endif
