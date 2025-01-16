// Fill out your copyright notice in the Description page of Project Settings.
#include "Online\SNOnlineSystem.h"

#include "SNDef.h"
#include "Utility/SNUtility.h"

#include "Online/SNOnlineCommonSetting.h"
#include "Online/SNOnlineSystemNull.h"
#include "Online/SNSessionSearchResult.h"

#if COMMONUSER_OSSV1
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionClient.h"
#include "OnlineSessionSettings.h"
#include "Online/SNOnlineSystemEOSV1.h"
#else
#include "Online/SNOnlineSystemEOSV2.h"
#endif

//----------------------------------------------------------------------//
//
//! @brief ログイン処理
//
//----------------------------------------------------------------------//
void USNOnlineSystem::Login(){
	// ログイン
	OnlineImpl->Login();

	SNPLUGIN_LOG(TEXT("Start Login"));
}

//----------------------------------------------------------------------//
//
//! @brief ログイン済みかどうかチェック
//
//! @retval true  ログイン済み
//! @retval false 未ログイン
//
//----------------------------------------------------------------------//
bool USNOnlineSystem::IsLoggedIn() const {
	return OnlineImpl->IsLoggedIn();
}

//----------------------------------------------------------------------//
//
//! @brief ホストとしてセッションを生成
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNOnlineSystem::HostSession(int ConnectionNum, FName Name){
	// リクエストが設定されていない場合は処理を終了
	if(HostSessionRequest == nullptr){
		
		SNPLUGIN_ERROR(TEXT("Host Session Request is nullptr."));
		
		return false;
	}
	
	HostSessionRequest->MaxPlayerNum = ConnectionNum;
	
	bool Result = OnlineImpl->HostSession(HostSessionRequest, Name);
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief セッションを検索
//
//----------------------------------------------------------------------//
bool USNOnlineSystem::FindSession(){
	return OnlineImpl->FindSession();
}

//----------------------------------------------------------------------//
//
//! @brief セッションに参加
//
//! @param SearchResult 参加するセッションの検索結果
//
//----------------------------------------------------------------------//
void USNOnlineSystem::JoinSession(const USNSessionSearchResult* SearchResult){
	OnlineImpl->JoinSession(SearchResult);
}

//----------------------------------------------------------------------//
//
//! @brief セッションを終了
//
//----------------------------------------------------------------------//
void USNOnlineSystem::KillSession(const FName& SessionName){
	// セッションを終了
	OnlineImpl->KillSession(SessionName);

	ConnectSession = nullptr;
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//----------------------------------------------------------------------//
void USNOnlineSystem::Initialize(){
	
	if(HostSessionRequestClass.IsNull() == false){
		// クラス情報をロード
		UClass* Class(HostSessionRequestClass.LoadSynchronous());
		
		SNPLUGIN_ASSERT(Class != nullptr, TEXT("Host Session Request Class is nullptr."));
		// ホスト生成リクエスト設定をロード
		HostSessionRequest = NewObject<USNOnlineHostSessionRequest>(this, Class, TEXT("HostSessionRequest"));
	}

	GEngine->NetworkFailureEvent.AddUObject(this, &USNOnlineSystem::OnNetworkFailureEvent);
#if COMMONUSER_OSSV1
	
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	FName PlatformName(OnlineSubsystem->GetSubsystemName());
	
	if(PlatformName == TEXT("NULL")){
		OnlineImpl = NewObject<USNOnlineSystemNull>();
	} else
	if(PlatformName == TEXT("EOS")){
		OnlineImpl = NewObject<USNOnlineSystemEOSV1>();
	}
#else
	OnlineImpl = NewObject<USNOnlineSystemEOSV2>();
#endif
	
	SNPLUGIN_ASSERT(OnlineImpl != nullptr, TEXT("Online Implementation is Invalid."));
	// 初期化処理
	OnlineImpl->Initialize(this);
}

const USNSessionSearchResult* USNOnlineSystem::GetConnectSession() const
{
	return ConnectSession;
}

void USNOnlineSystem::OnNetworkFailureEvent(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	if(OnNetworkFailure.IsBound())
	{
		OnNetworkFailure.Broadcast(World, NetDriver, FailureType, ErrorMessage);
	}
}


TArray<TObjectPtr<USNSessionSearchResult>> USNOnlineSystem::GetSessionSearchResultList() const {
	return OnlineImpl->GetSessionSearchResultList();
}

//----------------------------------------------------------------------//
//
//! @brief 最大プレイヤー数を取得
//
//! @retval 最大プレイヤー数
//
//----------------------------------------------------------------------//
int USNOnlineSystem::GetMaxPlayerNum() const {
	return (ConnectSession != nullptr) ? ConnectSession->GetConnectionNum() : -1;
}


void USNOnlineSystem::SetServerTravelMap(const FString& MapName){
	
	if(HostSessionRequest != nullptr){
		HostSessionRequest->MapName = MapName;
	}
}

void USNOnlineSystem::SetConnectSession(const USNSessionSearchResult* SessionSearchResult)
{
	ConnectSession = SessionSearchResult;
}

//----------------------------------------------------------------------//
//
//! @brief サーバーとしてマップ移動処理
//
//! @param World        ワールドへのポインタ
//! @param MapAssetName 移動するマップのアセット名
//
//----------------------------------------------------------------------//
void USNOnlineSystem::ServerTravel() const {
	
	UWorld* World(GetPrimaryWorld());
	// サーバーでない場合は処理をしない
	if(SNUtility::IsServer(World) == false){
		return;
	}
	
	if(HostSessionRequest == nullptr){
		return;
	}
	
	if(HostSessionRequest->MapName.IsEmpty() == true){
		return;
	}
	
	FString CombinedExtraArgs(TEXT(""));
	// LAN内での挙動かチェック(これを入れるとクライアント側がマップ移動できなくなるけど…)
	if(HostSessionRequest->OnlineMode == ESessionOnlineMode::LAN){
		CombinedExtraArgs += TEXT("?bIsLanMatch");
	}
	// オンラインでクライアントが移動するためにはこれが必要
	if(HostSessionRequest->OnlineMode != ESessionOnlineMode::Offline){
		CombinedExtraArgs += TEXT("?listen");
	}
	// 最終的なURLを生成
	FString TravelURL =FString::Printf(TEXT("%s%s"), *HostSessionRequest->MapName, *CombinedExtraArgs);

	SNPLUGIN_LOG(TEXT("ServerTravel To %s"), *TravelURL);
	// 移動
	World->ServerTravel(TravelURL);
}
