// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SNGameState.generated.h"

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief ゲームステートクラス
//
//! @note ホスト、クライアントともに存在します。
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNGameState : public AGameState
{
	GENERATED_BODY()

public:
	//! @{@name プレイヤーステートの追加処理
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	//! @}
	
	//! @{@name プレイヤーステートを削除
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	//! @}
	
	//! @{@name シームレスにレベル移動している際に呼ばれます。
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//! @}
	
	//! @{@name 接続数を取得
	int GetConnectPlayerNum() const ;
	//! @}
	
	//! @{@name ゲーム開始可能なプレイヤー数を取得
	int GetReadyToPlayerNum() const ;
	//! @}
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief 接続数を取得
//
//! @retval ロビーの接続数を取得
//
//----------------------------------------------------------------------//
FORCEINLINE int ASNGameState::GetConnectPlayerNum() const {
	return PlayerArray.Num();
}
