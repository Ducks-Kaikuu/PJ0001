// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNOnlineSystemInterface.h"
#include "UObject/NoExportTypes.h"
#include "SNOnlineSystemCommonV1.generated.h"

#if COMMONUSER_OSSV1
class USNSessionSearchResult;
#endif
//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief オンラインモードがOSSV1の共通処理クラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNOnlineSystemCommonV1 : public UObject, public ISNOnlineSystemInterface
{
	GENERATED_BODY()

public:
	//! @{@name 初期化処理
	virtual void Initialize(USNOnlineSystem* OnlineSystem) override;
	//! @}
	
	//! @{@name ログイン処理
	virtual void Login() override;
	//! @}
	
	//! @{@name 既にログインしているかチェック
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
	
	//! @{@name FindSessionの検索結果を取得
	virtual TArray<TObjectPtr<USNSessionSearchResult>> GetSessionSearchResultList() const override;
	//! @}
	
	//! @{@name セッションの作成に成功した際のデリゲート
	void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	//! @}
	
	
	
protected:
	
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	//! @{@name セッションへの参加が完了した際のでーりゲート
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	//! 
	
	//!< オーナーのオンラインシステム
	UPROPERTY()
	TObjectPtr<USNOnlineSystem> OwnerOnlineSystem = nullptr;
	
#if COMMONUSER_OSSV1
	//!< 検索用の設定
	TSharedPtr<class FOnlineSessionSearch> SearchSettings = nullptr;
	
	TArray<TObjectPtr<USNSessionSearchResult>> SessionSerchResultList;
#endif
	
	UPROPERTY()
	FString ConnectURL = TEXT("");
};
//! @}
//! @}
