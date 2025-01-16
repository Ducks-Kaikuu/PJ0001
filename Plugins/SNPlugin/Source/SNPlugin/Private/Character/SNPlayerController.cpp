// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SNPlayerController.h"

#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "Input/SNInputConfig.h"
#include "Utility/SNUtility.h"
#include "Input/SNInputManagerSubsystem.h"
#include "Online/SNOnlineSystem.h"
#include "Online/SNSessionSearchResult.h"

//----------------------------------------------------------------------//
//
//! @brief 終了処理
//
//! @param EndPlayReason 終了理由
//
//----------------------------------------------------------------------//
void ASNPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason){
	
	Super::EndPlay(EndPlayReason);
	
	USNInputManagerSubsystem* InputManagerSubsystem(SNUtility::GetSNInputManagerSubsystem());
	
	if(InputManagerSubsystem != nullptr){
		InputManagerSubsystem->RemoveAllInputContext(this);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 入力の初期化処理
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool ASNPlayerController::InitializeInput(){
	// 初期化フラグをON
	bInitializedInput = true;
	// ローカルプレイヤーかどうかチェック
	if(IsLocalController() == true){
		
		if(CurrentInputTypes.Num() > 0){
			// カレントに設定されているタイプを設定
			for(auto& Name:CurrentInputTypes){
				SetEnableInput(Name, false);
			}
		}
	} else {
		SNPLUGIN_LOG(TEXT("ASNPlayerController Initialize Input Called from RemoteController."));
	}
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 入力タイプを有効化
//
//! @param Name      有効化する入力タイプ名
//! @param bClearAll 現在設定されているインプットをクリアするかどうかのフラグ
//
//----------------------------------------------------------------------//
void ASNPlayerController::EnabledInputType(FName Name, bool bClearAll){
	// ローカルプレイヤーかどうかチェック
	if(IsLocalController() == true){
		// 初期化が済んでいる場合は即時設定
		if(bInitializedInput == true){
			// インプットを有効化
			bool Result = SetEnableInput(Name, bClearAll);
			
			if(Result == true){
				// クリアフラグが有効化チェック
				if(bClearAll){
					// カレントタイプをクリア
					CurrentInputTypes.Empty();
				}
				// 現在のインプットタイプに追加
				CurrentInputTypes.Add(Name);
				
				SNPLUGIN_LOG(TEXT("Current Input Type is %s."), *Name.ToString());
			}
		} else {
			// 現在のインプットタイプに追加
			CurrentInputTypes.Add(Name);
			
			SNPLUGIN_LOG(TEXT("Current Input Type Request to Add After Initialize.[%s]"), *Name.ToString());
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief 入力タイプを無効化
//
//! @param Name 無効化する入力タイプ名
//
//----------------------------------------------------------------------//
void ASNPlayerController::DisableInputType(FName Name){
	// ローカルプレイヤーかどうかチェック
	if(IsLocalController() == true){
		// 初期化が済んでいる場合は即時設定
		if(bInitializedInput == true){
			// 入力を無効化
			bool Result = SetDisableInput(Name);
			
			if(Result == true){
				// カレントタイプから削除
				CurrentInputTypes.Remove(Name);
				
				SNPLUGIN_LOG(TEXT("Remove Current Input Type[%s]."), *Name.ToString());
			}
		} else {
			// カレントタイプから削除
			CurrentInputTypes.Remove(Name);
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief 入力タイプを有効化
//
//! @param Name      有効化する入力タイプ名
//! @param bClearAll 現在設定されているインプットをクリアするかどうかのフラグ
//
//----------------------------------------------------------------------//
bool ASNPlayerController::SetEnableInput(FName Name, bool bClearAll){
	
	USNInputManagerSubsystem* InputManagerSubsystem(SNUtility::GetSNInputManagerSubsystem());
	// 指定されたマッピングを有効化
	return (InputManagerSubsystem != nullptr) ? InputManagerSubsystem->EnableInputMapping(this, Name, bClearAll) : false;
}

//----------------------------------------------------------------------//
//
//! @brief 入力タイプを無効化
//
//! @param Name 無効化する入力タイプ名
//
//----------------------------------------------------------------------//
bool ASNPlayerController::SetDisableInput(FName Name){
	
	USNInputManagerSubsystem* InputManagerSubsystem(SNUtility::GetSNInputManagerSubsystem());
	
	return (InputManagerSubsystem != nullptr) ? InputManagerSubsystem->DisableInputMapping(this, Name) : false;
}
