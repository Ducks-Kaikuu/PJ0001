// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/SNOnlineSession.h"

#include "SNDef.h"

void USNOnlineSession::RegisterOnlineDelegates()
{
	Super::RegisterOnlineDelegates();
}

void USNOnlineSession::ClearOnlineDelegates()
{
	Super::ClearOnlineDelegates();
}

//----------------------------------------------------------------------//
//
//! @brief ホストから切断された際のイベント
//
//! @param World     ワールド
//! @param NetDriver ネットドライバー
//
//----------------------------------------------------------------------//
void USNOnlineSession::HandleDisconnect(UWorld* World, class UNetDriver* NetDriver){
	
	SNPLUGIN_ASSERT(World != nullptr, TEXT("Failed to HandleDisconnect"));
	
	if(FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(World)){
		// 最後に移動したURL情報からListenとLANを削除
		// (これ…どういう意味があるんだろうか？)
		WorldContext->LastURL.RemoveOption( TEXT("Listen") );
		WorldContext->LastURL.RemoveOption( TEXT("LAN") );
		// 次に移動するマップを指定
		// (これ、もともとUE側の処理だとデフォルトマップに移動を指定してオプションに「?close」を設定してます)
		FString NextURL = FString::Printf(TEXT("%s%s"), *ReturnLevelAtDisconnection.GetAssetName(), TEXT("?listen"));
		// クライアントのトラベルに設定
		GEngine->SetClientTravel( World, *NextURL, TRAVEL_Absolute );
	} else
	{
		Super::HandleDisconnect(World, NetDriver);
	}
}

void USNOnlineSession::StartOnlineSession(FName SessionName)
{
	Super::StartOnlineSession(SessionName);
}

void USNOnlineSession::EndOnlineSession(FName SessionName)
{
	Super::EndOnlineSession(SessionName);
}

void USNOnlineSession::OnSessionUserInviteAccepted(const bool bWasSuccess, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	Super::OnSessionUserInviteAccepted(bWasSuccess, ControllerId, UserId, InviteResult);
}
