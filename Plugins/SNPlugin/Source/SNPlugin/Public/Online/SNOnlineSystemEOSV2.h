// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNOnlineSystemInterface.h"
#include "SNOnlineTypes.h"
#if !COMMONUSER_OSSV1
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/OnlineServices.h"
#endif

#include "SNOnlineSystemEOSV2.generated.h"

#if !COMMONUSER_OSSV1
namespace UE::Online
{
	struct FLobbyMemberJoined;
	struct FLobbyMember;
	struct FLobbyJoined;
	struct FLeaveSession;
}

namespace UE::Online
{
	struct FLeaveLobby;
}

class USNSessionSearchResult;

namespace UE::Online
{
	struct FLobbyMemberLeft;
	struct FFindLobbies;
	struct FJoinLobby;
	struct FLobby;
	struct FAuthLogin;
	struct FCreateLobby;
	struct FAccountInfo;
}
#endif

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief プラットフォーム設定がEOSの場合の通信処理
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNOnlineSystemEOSV2 : public UObject, public ISNOnlineSystemInterface
{
	GENERATED_BODY()

public:
	//! @{@name 初期化処理
	virtual void Initialize(USNOnlineSystem* OnlineSystem) override;
	//! @}
	
	//! @{@name ログイン処理
	virtual void Login() override;
	//! @}
	
	//! @{@name ログイン済みかどうかチェック
	virtual bool IsLoggedIn() const override;
	//! @}
	
	//! @{@name ホストとしてセッションを生成
	virtual bool HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name) override;
	//! @}
	
	//! @{@name セッションを検索
	virtual bool FindSession() override;
	//! @}
	
	//! @{@name セッションに参加
	virtual void JoinSession(const USNSessionSearchResult* SearchResult) override;
	//! @}
	
	//! @{@name セッションを終了
	virtual void KillSession(const FName& SessionName) override;
	//! @}
	
	virtual FUniqueNetIdRepl GetLocalUserNetId(APlayerController* PlayerController) const override;
	
	//! @{@name 
	virtual TArray<TObjectPtr<USNSessionSearchResult>> GetSessionSearchResultList() const override;
	//! @}

#if !COMMONUSER_OSSV1

#endif
	
private:

	virtual ELoginStatusType GetLocalUserLoginStatus(FPlatformUserId PlatformUser);
	
#if !COMMONUSER_OSSV1

	void BindDelegator();
	
	FUniqueNetIdRepl GetLocalUserNetId(FPlatformUserId PlatformUser) const ;
	
	bool IsRealPlatformUser(FPlatformUserId PlatformUser) const ;
	
	//! @{@name オンラインサービスを取得
	UE::Online::IOnlineServicesPtr GetOnlineServices() const;
	//! @}
	
	//! @{@name 認証インターフェイスを取得
	UE::Online::IAuthPtr GetAuth() const ;
	//! @}
	
	//! @{@name 指定されたユーザーのアカウント情報を取得
	TSharedPtr<UE::Online::FAccountInfo> GetOnlineServiceAccountInfo(FPlatformUserId PlatformUser) const ;
	//! @}
	
	//! @{@name EOSへのログインが完成した際のデリゲート
	void HandleUserLoginCompleted(const UE::Online::TOnlineResult<UE::Online::FAuthLogin>& Result, FPlatformUserId PlatformUser);
	//! @}
	
	//! @{@name セッションの生成が終了した際に呼ばれるデリゲート
	void HandleUserCreateSessionCompleted(const UE::Online::TOnlineResult<UE::Online::FCreateLobby>& Result, FName SessionName);
	//! @}
	
	//! @{@name セッションの検索が完了した際のデリゲート
	void HandleUserFindSessionComplete(const UE::Online::TOnlineResult<UE::Online::FFindLobbies>& FindResult);
	//! @}
	
	//! @{@name セッションに参加が完了した際のデリゲート
	void HandleUserJoinSessionComplete(const UE::Online::TOnlineResult<UE::Online::FJoinLobby>& JoinResult, const USNSessionSearchResult* SearchResult);
	//! @}

	void HandleMemberLeftLobby(const UE::Online::FLobbyMemberLeft& LeftResult);
	
	//! @{@name ロビーからの離脱が完了した際のデリゲート
	void HandleUserLeaveLobby(const UE::Online::TOnlineResult<UE::Online::FLeaveLobby>& Result);
	//! @}
	
	//! @{@name ロビーにメンバーが参加した際のイベント
	void HandleLobbyJoined(const UE::Online::FLobbyMemberJoined& JoinedMember);
	//! @}
	
	//!< セッションを作成するためにLocalPlayerを必要としない専用サーバーかどうか。
	bool bIsDedicatedServer = false;
	
	TArray<TSharedRef<const UE::Online::FLobbyMember>> LobbyMembers;

	UE::Online::FOnlineEventDelegateHandle JoinedDelegateHandle;

	UE::Online::FOnlineEventDelegateHandle MemberLeftDelegateHandle;

#endif
	
	UPROPERTY()
	TArray<TObjectPtr<USNSessionSearchResult>> SessionSerchResultList;
	
	//!< オーナーのオンラインシステム
	UPROPERTY()
	TObjectPtr<USNOnlineSystem> OwnerOnlineSystem = nullptr;
	
	UPROPERTY()
	FString ConnectURL = TEXT("");
};
//! @}
//! @}
