// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNSessionSearchResult.generated.h"

// OSSバージョンによりタイプを定義
#if COMMONUSER_OSSV1
#include "OnlineSessionSettings.h"
typedef FOnlineSessionSearchResult SessionSearchResult;
#else
namespace UE::Online
{
	struct FLobby;
}

typedef TSharedPtr<const UE::Online::FLobby> SessionSearchResult;
#endif
//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief セッションの検索結果プロキシークラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNSessionSearchResult : public UObject
{
	GENERATED_BODY()
public:
	//! @{@name 代入演算子
	USNSessionSearchResult& operator=(const SessionSearchResult& InResult);
	//! @}
	
	//! @{@name 設定関数
	void SetSessionSearchResult(const SessionSearchResult& InResult);
	//! @}
	
	//! @{@name リザルトが有効かチェック
	bool IsValid() const ;
	//! @}
	
	//! @{@name セッション名を取得
	FName GetSessionName() const ;
	//! @}
	
	//! @{@name 最大接続数を取得
	int GetConnectionNum() const ;
	//! @}
	
	//! @{@name セッションの検索結果を取得
	const SessionSearchResult& Get() const ;
	//! @}
	
private:
	//!< 検索結果
	SessionSearchResult Result;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief セッションの検索結果を取得
//
//! @retval セッションの検索結果
//
//----------------------------------------------------------------------//
FORCEINLINE const SessionSearchResult& USNSessionSearchResult::Get() const {
	return Result;
}
