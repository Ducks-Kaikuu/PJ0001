// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNOnlineSystemCommonV1.h"
#include "SNOnlineSystemEOSV1.generated.h"

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief プラットフォーム設定がEOSの場合の通信処理
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNOnlineSystemEOSV1 : public USNOnlineSystemCommonV1
{
	GENERATED_BODY()
	
public:
	
	//! @{@name 初期化処理
	virtual void Initialize(USNOnlineSystem* OnlineSystem) override ;
	//! @}
	
	//! @{@name ログイン処理
	virtual void Login() override;
	//! @}
	
	//! @{@name ホストとしてセッションを生成
	virtual bool HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name) override;
	//! @}
	
private:
	//! @{@name ログインに成功した際のデリゲート
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	//! @}
	
	//!< セッションを作成するためにLocalPlayerを必要としない専用サーバーかどうか。
	bool bIsDedicatedServer = false;
};
//! @}
//! @}
