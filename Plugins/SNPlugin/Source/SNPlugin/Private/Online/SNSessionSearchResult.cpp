// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/SNSessionSearchResult.h"

#include "Online/OnlineSessionNames.h"
#if !COMMONUSER_OSSV1
#include "Online/Lobbies.h"
#endif

//----------------------------------------------------------------------//
//
//! @brief 代入演算子
//
//! @param InResult 代入元
//
//! @retval 自分への参照
//
//----------------------------------------------------------------------//
USNSessionSearchResult& USNSessionSearchResult::operator=(const SessionSearchResult& InResult){
	
	if(&Result != &InResult){
		Result = InResult;
	}
	
	return *this;
}

//----------------------------------------------------------------------//
//
//! @brief 設定関数
//
//! @param InResult セッションの検索結果
//
//----------------------------------------------------------------------//
void USNSessionSearchResult::SetSessionSearchResult(const SessionSearchResult& InResult){
	Result = InResult;
}

//----------------------------------------------------------------------//
//
//! @brief リザルトが有効かチェック
//
//! @retval true  有効
//! @retval false 無効
//
//----------------------------------------------------------------------//
bool USNSessionSearchResult::IsValid() const {
	return Result.IsValid();
}

//----------------------------------------------------------------------//
//
//! @brief セッション名を取得
//
//! @retval セッション名
//
//----------------------------------------------------------------------//
FName USNSessionSearchResult::GetSessionName() const {
	
	FString SessionName(TEXT(""));
	
#if COMMONUSER_OSSV1
	
	const FOnlineSessionSetting& Setting(Result.Session.SessionSettings.Settings[SEARCH_KEYWORDS]);
	
	SessionName = Setting.Data.ToString();
	
#else
	if(Result->Attributes.Find(SETTING_SESSION_TEMPLATE_NAME)){
		SessionName = Result->Attributes[SETTING_SESSION_TEMPLATE_NAME].GetString();
	}
#endif
	return *SessionName;
}

//----------------------------------------------------------------------//
//
//! @brief 最大接続数を取得
//
//! @retval 最大接続数
//
//----------------------------------------------------------------------//
int USNSessionSearchResult::GetConnectionNum() const {
#if COMMONUSER_OSSV1
	return Result.Session.NumOpenPublicConnections;
#else
	return Result->MaxMembers;
#endif
}


