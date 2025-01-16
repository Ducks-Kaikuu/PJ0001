// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SNGameState.h"

#include "Character/SNPlayerState.h"
#include "Character/Base/SNPlayerBase.h"
#include "GameFramework/PlayerState.h"

//----------------------------------------------------------------------//
//
//! @brief プレイヤーステートの追加処理
//
//! @param PlayerState プレイヤーステート
//
//! @note 接続が構築されるとホストのGameStateにクライアントのPlayerStateが追加されます。
//
//----------------------------------------------------------------------//
void ASNGameState::AddPlayerState(APlayerState* PlayerState){
	Super::AddPlayerState(PlayerState);
}

//----------------------------------------------------------------------//
//
//! @brief プレイヤーステートを削除
//
//! @param PlayerState プレイヤーステート
//
//! @note 接続が切断された場合に切断されたPlayerStateが削除されます。
//
//----------------------------------------------------------------------//
void ASNGameState::RemovePlayerState(APlayerState* PlayerState){
	Super::RemovePlayerState(PlayerState);
}

//----------------------------------------------------------------------//
//
//! @brief シームレスにレベル移動している際に呼ばれます。
//
//! @param bToTransitionMap true : 遷移マップへ移動 / false : ターゲットマップへ移動
//
//----------------------------------------------------------------------//
void ASNGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap){
	// 非アクティブやボットのプレイヤーステートを削除
	for(int32 i=PlayerArray.Num()-1; i>=0; i--){
		
		APlayerState* PlayerState = PlayerArray[i];
		
		if((PlayerState != nullptr) && ((PlayerState->IsABot() == true) || (PlayerState->IsInactive() == true))){
			RemovePlayerState(PlayerState);
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief ゲーム開始可能なプレイヤー数を取得
//
//! @retval ゲーム開始可能なプレイヤー数
//
//! @note サーバー以外は0を返します
//
//----------------------------------------------------------------------//
int ASNGameState::GetReadyToPlayerNum() const {
	// サーバー以外は0を返します。
	if(HasAuthority() == false){
		return 0;
	}
	
	int num = 0;
	
	for(int32 i=0 ; i<PlayerArray.Num() ; i++){
		// プレイヤーステートを取得
		ASNPlayerState* State(Cast<ASNPlayerState>(PlayerArray[i]));
		
		if(State != nullptr){
			// プレイヤーを取得
			ASNPlayerBase* Player = Cast<ASNPlayerBase>(State->GetPawn());
			// ゲームが開始可能かチェック
			if((Player != nullptr) && (Player->IsReadyToPlay() == true)){
				++num;
			}
		}
	}
	
	return num;
}
