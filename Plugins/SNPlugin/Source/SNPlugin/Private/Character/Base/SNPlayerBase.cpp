// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Base/SNPlayerBase.h"

#include "SNDef.h"
#include "Action/SNActionBase.h"
#include "Character/SNPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/SNInputConfig.h"
#include "Input/SNInputManagerSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Utility/SNUtility.h"

//----------------------------------------------------------------------//
//
//! @brief 入力の可否を設定
//
//! @param InAvailability true : 入力を有効化 / false : 入力を無効化
//
//----------------------------------------------------------------------//
void ASNPlayerBase::SetInputAvailability(bool InAvailability){
	
	APlayerController* PlayerController(Cast<APlayerController>(GetController()));
	
	if(InAvailability == true){
		// 入力を有効化
		EnableInput(PlayerController);
	} else {
		// 入力を無効化
		DisableInput(PlayerController);
	}
}

//----------------------------------------------------------------------//
//
//! @brief ポーンが再起動されたタイミングで呼ばれます。
//
//! @note デフォルトでは、すべてのポーンに対してサーバー上で呼び出され、
//!       プレイヤーポーンに対しては所有クライアント上で呼び出されます。
//!     * サブクラスでオーバーライドして、データがロード/複製されるまで再起動の通知を遅らせることができます。
//
//----------------------------------------------------------------------//
void ASNPlayerBase::NotifyRestarted(){
	
	Super::NotifyRestarted();
	// ローカルプレイヤーの場合はこのタイミングでインプットコンフィグの初期化を行う
	// (InputComponentが生成された後に実行しないとアクションのバインドができないため)
	if(IsLocallyControlled() == true){
		
		InitializeInputConfig();
		
		SNPLUGIN_LOG(TEXT("Input Config Initialize in NotifyRestarted.[%s]"), *GetName());
	}
	
	ASNPlayerController* PlayerController(Cast<ASNPlayerController>(Controller));
	
	if(PlayerController != nullptr){
		// 入力の初期化処理
		PlayerController->InitializeInput();
		
		SNPLUGIN_LOG(TEXT("Setup Player Input Component."));
	}
}

//----------------------------------------------------------------------//
//
//! @brief BeginPlayがコールされたことをサーバーに通知
//
//----------------------------------------------------------------------//
void ASNPlayerBase::ReadyToPlayOnServer_Implementation(){
	bBeginPlay = true;
}


void ASNPlayerBase::PossessedBy(AController* NewController){
	
	Super::PossessedBy(NewController);
	
	bPossessed = true;

	
}

//----------------------------------------------------------------------//
//
//! @brief ゲーム開始処理
//
//----------------------------------------------------------------------//
void ASNPlayerBase::BeginPlay(){
	
	Super::BeginPlay();
	// ローカルプレイヤーでない場合はこのタイミングでインプットコンフィグの初期化をw行う
	// (RemoteプレイヤーはNotifyRestartedが呼ばれない。)
	// ただ、OnServerやOnNetMulticastでアクションを実行する必要がない場合はそもそもこの処理は不要
	if(IsLocallyControlled() == false){
		
		InitializeInputConfig();
		
		SNPLUGIN_LOG(TEXT("Input Config Initialize in BeginPlay.[%s]"), *GetName());
	}
	
	if(IsLocallyControlled() == true){
		// サーバー側にBeginPlayが呼ばれたことを通知
		ReadyToPlayOnServer();
	}

	SetNetCullDistanceSquared(500000.0f * 500000.0f);
	
	bAlwaysRelevant = true;
	
	NetPriority = 5.0f;

	SetNetUpdateFrequency(60.0f);

	SetMinNetUpdateFrequency(30.0f);

	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;

	ASNPlayerController* PlayerController(Cast<ASNPlayerController>(Controller));

	if(PlayerController != nullptr)
	{
		PlayerController->bAutoManageActiveCameraTarget = false;
	}
}

//----------------------------------------------------------------------//
//
//! @brief インプットコンフィグの初期化処理
//
//----------------------------------------------------------------------//
void ASNPlayerBase::InitializeInputConfig(){
	// 初期化処理
	for(auto& InputConfig:InputConfigMap){
		InputConfig.Value->InitializeInput(InputConfig.Key, this);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 終了処理
//
//! @param EndPlayReason 終了理由
//
//----------------------------------------------------------------------//
void ASNPlayerBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
	
	Super::EndPlay(EndPlayReason);
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

	SNPLUGIN_LOG(TEXT("ASNPlayerBase Endplay is done.[%s]"), *GetName());
}

//----------------------------------------------------------------------//
//
//! @brief 入力された際に呼ばれるアクションを追加
//
//! @param Tag   アクション用ゲームタグ
//! @param Action アクションインスタンス
//
//----------------------------------------------------------------------//
void ASNPlayerBase::AddInputAction(const FGameplayTag& Tag, USNActionBase* Action)
{
	InputActionMap.Add(Tag, Action);
}

//----------------------------------------------------------------------//
//
//! @brief アクションを取得
//
//! @param  Tag   アクション用ゲームタグ
//
//! @retval アクション
//
//----------------------------------------------------------------------//
USNActionBase* ASNPlayerBase::GetAction(const FGameplayTag& Tag)
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
int ASNPlayerBase::GetActionNum() const {
	return InputActionMap.Num();
}

//----------------------------------------------------------------------//
//
//! @brief 入力からアクション実行用のレプリケーション(サーバー)
//
//! @param Name       アクション名
//! @param InputValue 入力情報
//! @param Type       入力タイプ
//
//----------------------------------------------------------------------//
void ASNPlayerBase::ExecuteInputAction_OnServer_Implementation(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type){
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
void ASNPlayerBase::ExecuteInputAction_OnMulticast_Implementation(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type){
	// サーバーの場合は処理を回さない
	if(SNUtility::IsServer(GetPrimaryWorld()) != true){
		// アクションを実行
		ExexuteInputActionImplement(Tag, InputValue, Type);
	}
}
