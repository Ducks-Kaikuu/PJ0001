// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/SNUserWidgetBase.h"
#include "SNMatchingTestRoomItem.generated.h"

class USNSessionSearchResult;
class UTextBlock;
class USNButton;
/**
 * 
 */
UCLASS()
class SNMULTITEST_API USNMatchingTestRoomItem : public USNUserWidgetBase
{
	GENERATED_BODY()
	
public:
	
	//! @{@name 初期化処理
	bool Initialize() override;
	//! @}
	
	//! @{@name ルーム情報を設定
	void SetRoom(const FString& Name, int Num, USNSessionSearchResult* SearchResult);
	//! @}
	
	//! @{@name 接続用のボタンを取得
	USNButton* GetJoinButton();
	//! @}
	
	//! @{@name セッション名を取得
	FString GetSessionName() const ;
	//! @}
	
	//! @{@name セッションの検索結果
	const USNSessionSearchResult* GetSearchResult() const;
	//! @}
	
private:
	
	//!< 接続用のボタン
	UPROPERTY()
	TObjectPtr<USNButton> JoinButton = nullptr;
	
	//!< セッション名
	UPROPERTY()
	TObjectPtr<UTextBlock> SessionName = nullptr;
	
	//!< 接続数
	UPROPERTY()
	TObjectPtr<UTextBlock> ConnectionNum = nullptr;
	
	//!< セッションの検索結果
	UPROPERTY()
	TObjectPtr<USNSessionSearchResult> Element = nullptr;
};

//----------------------------------------------------------------------//
//
//! @brief 接続用のボタンを取得
//
//! @retval ボタンへのポインタ
//
//----------------------------------------------------------------------//
FORCEINLINE USNButton* USNMatchingTestRoomItem::GetJoinButton(){
	return JoinButton;
}

//----------------------------------------------------------------------//
//
//! @brief セッションの検索結果を取得
//
//! @retval セッションの検索結果
//
//----------------------------------------------------------------------//
inline const USNSessionSearchResult* USNMatchingTestRoomItem::GetSearchResult() const {
	return Element;
}


