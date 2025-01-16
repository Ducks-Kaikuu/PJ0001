// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UObject/Interface.h"
#include "SNOnlineSystemInterface.generated.h"

class USNSessionSearchResult;
class USNOnlineSystem;
class USNOnlineHostSessionRequest;

//!@{@defgroup オンライン
//!@{
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USNOnlineSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SNPLUGIN_API ISNOnlineSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	//! @{@name 初期化処理
	virtual void Initialize(USNOnlineSystem* OnlineSystem) = 0;
	//! @}
	
	//! @{@name ログイン処理
	virtual void Login() = 0;
	//! @}
	
	//! @{@name ログイン済みかどうかチェック
	virtual bool IsLoggedIn() const = 0;
	//! @}
	
	//! @{@name ホストとしてセッションを生成
	virtual bool HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name) = 0;
	//! @}
	
	//! @{@name セッションを検索
	virtual bool FindSession() = 0;
	//! @}
	
	//! @{@name セッションに参加
	virtual void JoinSession(const USNSessionSearchResult* SearchResult) = 0;
	//! @}
	
	//! @{@name セッションを終了
	virtual void KillSession(const FName& SessionName) = 0;
	//! @}
	
	virtual FUniqueNetIdRepl GetLocalUserNetId(APlayerController* PlayerController) const = 0;

	virtual TArray<TObjectPtr<USNSessionSearchResult>> GetSessionSearchResultList() const { return TArray<TObjectPtr<USNSessionSearchResult>>(); } 
};
//! @}
//! @}
