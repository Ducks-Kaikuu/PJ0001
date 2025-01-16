// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Base/SNWheeledVehiclePlayerBase.h"

#include "SNDef.h"
#include "Action/SNActionBase.h"
#include "Character/SNPlayerController.h"
#include "Input/SNInputConfig.h"
#include "Input/SNInputManagerSubsystem.h"
#include "Utility/SNUtility.h"

//----------------------------------------------------------------------//
//
//! @brief プレイヤーのリスタート処理
//
//! @note 接続後、InputComponentなどの初期化も終わったタイミングで呼ばれます。
//        ホストではないプレイヤーにInputComponentは存在しないので、注意。
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::NotifyRestarted(){
	
	Super::NotifyRestarted();
	
	ASNPlayerController* PlayerController(Cast<ASNPlayerController>(Controller));
	
	if(PlayerController != nullptr){
		// 入力の初期化処理
		PlayerController->InitializeInput();
		
		SNPLUGIN_LOG(TEXT("NotifyRestarted."));
	}
	// ローカルプレイヤーの場合はこのタイミングでインプットコンフィグの初期化を行う
	// (InputComponentが生成された後に実行しないとアクションのバインドができないため)
	if(IsLocallyControlled() == true){
		
		InitializeInputConfig();
		
		SNPLUGIN_LOG(TEXT("Input Config Initialize in NotifyRestarted.[%s]"), *GetName());
	}}

//----------------------------------------------------------------------//
//
//! @brief ゲーム開始処理
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::BeginPlay(){
	
	Super::BeginPlay();
	// ローカルプレイヤーでない場合はこのタイミングでインプットコンフィグの初期化をw行う
	// (RemoteプレイヤーはNotifyRestartedが呼ばれない。)
	// ただ、OnServerやOnNetMulticastでアクションを実行する必要がない場合はそもそもこの処理は不要
	if(IsLocallyControlled() == false){
		
		InitializeInputConfig();
		
		SNPLUGIN_LOG(TEXT("Input Config Initialize in BeginPlay.[%s]"), *GetName());
	}
}

//----------------------------------------------------------------------//
//
//! @brief インプットコンフィグの初期化処理
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::InitializeInputConfig(){
	// 初期化処理
	for(auto& InputConfig:InputConfigMap){
		InputConfig.Value->InitializeInput(InputConfig.Key, this);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 終了処理
//
//! @param EndPlayReason 終了の理由
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
	
	Super::EndPlay(EndPlayReason);
	
	for(auto& Action:InputActionMap){
		// アクションを破棄
		Action.Value->ConditionalBeginDestroy();
	}
	// アクションリストをリセット
	InputActionMap.Reset();
	
	ASNPlayerController* PlayerController(Cast<ASNPlayerController>(Controller));
	
	if(PlayerController != nullptr){
		// 入力管理用マネージャーを取得
		USNInputManagerSubsystem* InputManagerSubsystem(SNUtility::GetSNInputManagerSubsystem());
		
		for(auto& InputConfig:InputConfigMap){
			// 入力マッピングを無効化
			InputManagerSubsystem->DisableInputMapping(PlayerController, InputConfig.Key);
			// リソースを解放
			InputConfig.Value->Release();
		}
	}
	// アクションをクリア
	InputActionMap.Empty();
	
	SNPLUGIN_LOG(TEXT("ASNWheeledVehiclePlayerBase Endplay is done."))
}


//----------------------------------------------------------------------//
//
//! @brief 入力された際に呼ばれるアクションを追加
//
//! @param Tag   アクション用ゲームタグ
//! @param Action アクションへのポインタ
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::AddInputAction(const FGameplayTag& Tag, USNActionBase* Action)
{
	InputActionMap.Add(Tag, Action);
}

//----------------------------------------------------------------------//
//
//! @brief アクションを取得
//
//! @param Tag アクション用ゲームタグ
//
//! @retval アクションへのポインタ
//
//----------------------------------------------------------------------//
USNActionBase* ASNWheeledVehiclePlayerBase::GetAction(const FGameplayTag& Tag)
{
	USNActionBase* Action = nullptr;
	
	if(InputActionMap.Contains(Tag) == true){
		Action = InputActionMap[Tag];
	}
	
	return Action;
}

//----------------------------------------------------------------------//
//
//! @brief 管理しているアクション数を取得
//
//! @retval 管理しているアクション数
//
//----------------------------------------------------------------------//
int ASNWheeledVehiclePlayerBase::GetActionNum() const {
	return InputActionMap.Num();
}

//----------------------------------------------------------------------//
//
//! @brief 入力からアクション実行用のレプリケーション
//
//! @param Name       アクション名
//! @param InputValue 入力値
//! @param Type       入力タイプ
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::ExecuteInputAction_OnServer_Implementation(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type){
	// アクションを実行
	ExexuteInputActionImplement(Tag, InputValue, Type);
}

//----------------------------------------------------------------------//
//
//! @brief 入力からアクション実行用のレプリケーション(クライアント)
//
//! @param Name       アクション名
//! @param InputValue 入力情報
//! @param Type       入力タイプ
//
//----------------------------------------------------------------------//
void ASNWheeledVehiclePlayerBase::ExecuteInputAction_OnMulticast_Implementation(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type){
	// サーバーの場合は処理を回さない
	if(SNUtility::IsServer(GetPrimaryWorld()) != true){
		// アクションを実行
		ExexuteInputActionImplement(Tag, InputValue, Type);
	}
}
