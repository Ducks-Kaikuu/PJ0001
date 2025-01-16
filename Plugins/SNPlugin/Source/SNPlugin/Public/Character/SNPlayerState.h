// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SNPlayerState.generated.h"

//!@{@defgroup オンライン
//!@{
/**
 スコア、キル数、デス数、Pingなどの情報を保持し、エンジン側でレプリケートされています。\n
 また、プレイヤー名やプレイヤーIDなども保持しています。\n
 動作未確認ではありますが、一度切断され、再接続した際の復帰もPlayerStateで行われます。
*/
//----------------------------------------------------------------------//
//
//! @brief プレイヤーの状態管理クラス
//
//! @note ホスト、クライアントともに存在します。
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASNPlayerState(const FObjectInitializer& ObjectInitializer);
private:
	
};
//! @}
//! @}
