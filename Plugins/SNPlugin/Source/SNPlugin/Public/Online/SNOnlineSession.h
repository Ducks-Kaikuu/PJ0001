// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/OnlineSession.h"
#include "SNOnlineSession.generated.h"

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief オンラインセッションクラス
//
//! @note ホストとのセッションが切断された際にDefaultMapに移動させたくない場合に使用するクラス
//         SNGameInstanceとセットで使用してください。
//
//----------------------------------------------------------------------//
UCLASS(Blueprintable)
class SNPLUGIN_API USNOnlineSession : public UOnlineSession
{
	GENERATED_BODY()

public:
	
	virtual void RegisterOnlineDelegates() override;
	
	virtual void ClearOnlineDelegates() override;
	
	//! @{@name ホストから切断された際のイベント
	virtual void HandleDisconnect(UWorld* World, class UNetDriver* NetDriver) override;
	//! @}
	
	virtual void StartOnlineSession(FName SessionName) override;
	
	virtual void EndOnlineSession(FName SessionName) override;
	
	virtual void OnSessionUserInviteAccepted(const bool bWasSuccess, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult) override;
	
private:
	UPROPERTY(EditAnywhere, Category = Online)
	TSoftObjectPtr<UObject> ReturnLevelAtDisconnection; 
};
//! @}
//! @}
