// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Core/Connection/NetEnums.h"
#if COMMONUSER_OSSV1
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineError.h"
#else
#include "Online/OnlineAsyncOpHandle.h"
#endif

#include "SNOnlineSystem.generated.h"

class USNSessionSearchResult;
class ISNOnlineSystemInterface;
class IOnlineSubsystem;
class USNOnlineHostSessionRequest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSNCompleteLoggedIn, bool, bWasSuccessful, const FString&, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSNCompleteHostSession, FName, InSessionName, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSNCompleteFindSession, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSNCompleteJoinSession, FName, InSessionName, bool, bResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSNNetworkFailure, UWorld*, World, UNetDriver*, NetDriver, ENetworkFailure::Type, FailureType, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSNCompleteLeaveSession, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSNMemberLeftLobby);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSNMemberJoineLotty);

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief オンライン処理
//
//----------------------------------------------------------------------//
UCLASS(Blueprintable)
class SNPLUGIN_API USNOnlineSystem : public UObject
{
	GENERATED_BODY()

public:
	
	//! @{@name 初期化処理
	void Initialize();
	//! @}
	
	//! @{@name ログイン処理
	void Login();
	//! @}
	
	//! @{@name 既にログイン済みかどうかチェック
	bool IsLoggedIn() const ;
	//! @}
	
	//! @{@name ホストとしてセッションを生成
	bool HostSession(int ConnectionNum=4, FName Name=FName(TEXT("Session")));
	//! @}
	
	//! @{@name セッションを検索
	bool FindSession();
	//! @}
	
	//! @{@name セッションに参加
	void JoinSession(const USNSessionSearchResult* SearchResult);
	//! @}
	
	//! @{@name セッションを終了
	void KillSession(const FName& SessionName);
	//! @}
	
	//! @{@name 最大コネクト数を取得
	int GetMaxPlayerNum() const ;
	//! @}
	
	//! @{@name サーバーとしてマップ移動処理
	void ServerTravel() const ;
	//! @}
	
	void SetServerTravelMap(const FString& MapName);

	void SetConnectSession(const USNSessionSearchResult* SessionSearchResult);
	
	TArray<TObjectPtr<USNSessionSearchResult>> GetSessionSearchResultList() const ;
	
	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNCompleteLoggedIn OnCompleteLoggedIn;
	
	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNCompleteHostSession OnCompleteHostSession;
	
	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNCompleteFindSession OnCompleteFindSession;
	
	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNCompleteJoinSession OnCompleteJoinSession;

	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNNetworkFailure OnNetworkFailure;

	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNCompleteLeaveSession OnCompleteLeaveSession;

	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNMemberLeftLobby OnMemberLeftLobby;

	UPROPERTY(BlueprintAssignable, Category = "Online|Session")
	FSNMemberJoineLotty OnMemberJoineLotty;
	
	const USNSessionSearchResult* GetConnectSession() const ;
	
#if WITH_EDITORONLY_DATA
	//! @{@name 開発時に使用するオンライントークンを設定
	void	SetOnlineToken(const FName& Token);
	//! @}
	
	//! @{@name 開発時に使用するオンライントークンを取得
	const FName& GetOnlineToken() const ;
	//! @}
	
	//! @{@name 有効なオンライントークンが設定されているかチェック
	bool IsEnableOnlineToken() const ;
	//! @}
#endif
	
private:

	void OnNetworkFailureEvent(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage);
	
#if WITH_EDITORONLY_DATA
	//!< 開発時に使用するオンライントークン
	UPROPERTY()
	FName OnlineToken=NAME_None;
#endif // WITH_EDITORONLY_DATA
	
	UPROPERTY(EditAnywhere, Category="Online|HostSession")
	TSoftClassPtr<USNOnlineHostSessionRequest> HostSessionRequestClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<USNOnlineHostSessionRequest> HostSessionRequest = nullptr;
	
	UPROPERTY()
	TScriptInterface<ISNOnlineSystemInterface> OnlineImpl = nullptr;
	
	UPROPERTY()
	TObjectPtr<const USNSessionSearchResult> ConnectSession = nullptr;
	
	UPROPERTY()
	int MaxPlayerNum = -1;
};

#if WITH_EDITORONLY_DATA
//----------------------------------------------------------------------//
//
//! @brief 開発時に使用するオンライントークンを設定
//
//! @param Token オンライントークン(EOS_DevAuthToolで設定したトークン)
//
//----------------------------------------------------------------------//
FORCEINLINE void USNOnlineSystem::SetOnlineToken(const FName& Token){
	OnlineToken = Token;
}

//----------------------------------------------------------------------//
//
//! @brief 開発時に使用するオンライントークンを取得
//
//! @retval オンライントークン(EOS_DevAuthToolで設定したトークン)
//
//----------------------------------------------------------------------//
FORCEINLINE const FName& USNOnlineSystem::GetOnlineToken() const {
	return OnlineToken;
}

//----------------------------------------------------------------------//
//
//! @brief 有効なオンライントークンが設定されているかチェック
//
//! @retval true : 有効なトークン / false : 無効なトークン
//
//----------------------------------------------------------------------//
FORCEINLINE bool USNOnlineSystem::IsEnableOnlineToken() const {
	return (OnlineToken.IsNone() == false) ? true : false;;
}
#endif
//! @}
//! @}
