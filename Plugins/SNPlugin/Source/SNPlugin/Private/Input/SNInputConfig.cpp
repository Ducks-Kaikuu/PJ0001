// Fill out your copyright notice in the Description page of Project Settings.
#include "Input/SNInputConfig.h"
#include "SNDef.h"
#include "Utility/SNUtility.h"
#include "Input/SNInputManagerSubsystem.h"
#include "Action/SNActionBase.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Character/Base/SNPlayablePawnInterface.h"


//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//! @param ObjectInitializer イニシャライザ
//
//----------------------------------------------------------------------//
USNInputConfig::USNInputConfig(const FObjectInitializer& ObjectInitializer):
 Super(ObjectInitializer)
{
	InputActionList.Empty();
}

//----------------------------------------------------------------------//
//
//! @brief 入力周りの初期化処理
//
//! @param Name        コンフィグ名
//! @param OwnerObject オーナーとなるオブジェクト
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNInputConfig::InitializeInput(FName Name, UObject* OwnerObject){

	SNPLUGIN_ASSERT(OwnerObject != nullptr, TEXT("Should set Input Action Owner."));
	
	ISNPlayablePawnInterface* PlayablePawn = Cast<ISNPlayablePawnInterface>(OwnerObject);
	// オーナーを設定
	OwnerActor = OwnerObject;
	// コンフィグ名を設定
	ConfigName = Name;
	// リストを空にする
	ActionList.Empty();
	// インプットマッピングコンテキストをロード
	// (@@Note : ヒッチが発生する場合は非同期にするか検討)
	IMCInstance = InputMappingContext.LoadSynchronous();
	// ロードに失敗した場合はアサート
	SNPLUGIN_ASSERT(IMCInstance != nullptr, TEXT("InputMappingContext is nullptr."));	
	
	APawn* OwnerPawn(Cast<APawn>(OwnerObject));
	// ローカルプレイヤー以外はContextMappingへの登録は行わない
	if((OwnerPawn != nullptr) && (OwnerPawn->Controller != nullptr) && (OwnerPawn->Controller->IsLocalController() == true)){
		// 入力マネージャーサブシステムを取得
		USNInputManagerSubsystem* InputManagerSubsystem(SNUtility::GetSNInputManagerSubsystem());
		// 取得できない場合はアサート
		SNPLUGIN_ASSERT(InputManagerSubsystem != nullptr, TEXT("InputManagerSubsystem is nullptr."));
		// コンテキストを追加
		InputManagerSubsystem->AddInputContext(ConfigName, IMCInstance);
	}
	
	UEnhancedInputComponent* InputComponent = nullptr;
	
	if(OwnerPawn != nullptr){
		InputComponent = Cast<UEnhancedInputComponent>(OwnerPawn->InputComponent);
	}
	
	for(auto& action:InputActionList){
		// クラス情報をロード
		UObject* Object = action.ActionClass.LoadSynchronous();
		
		if(Object != nullptr){
			
			UClass* ClassPtr = Cast<UClass>(Object);
			
			if(ClassPtr != nullptr){
				
				USNActionBase* Action = NewObject<USNActionBase>(OwnerActor, ClassPtr);
				
				if(Action != nullptr){
					// ※@@Note : ロードの順番が登録した順番として保証されてない場合は見直しが必要
					FSNInputAction& Input(action);
					// アクション名を設定
					// Action->SetActionName(*(Input.InputAction->GetName()));
					if (Action->GetActionTag().IsValid() == false)
					{
						Action->SetActionTag(Input.InputTag);	
					}

					if (Action->GetExclusiveActionTag().IsValid() == false)
					{
						Action->SetExclusiveActionTag(Input.ExclusiveTags);
					}
					
					if(PlayablePawn != nullptr){

						PlayablePawn->AddInputAction(Input.InputTag, Action);
						
						SNPLUGIN_LOG(TEXT("AddInputAction : %s"), *Action->GetActionTag().ToString());
					}  else {
						SNPLUGIN_LOG(TEXT("Failed to AddInputAction."));
					}
					// 入力コンポーネントにバインド
					Action->Initialize(InputComponent, Input.InputAction.Get(), OwnerActor);
					// リストに登録
					ActionList.Add(Action);
				}
			}
		}
	}
	
	SNPLUGIN_LOG(TEXT("Config Initialize is done.[%s]"), *OwnerObject->GetName());
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 解放処理
//
//----------------------------------------------------------------------//
void USNInputConfig::Release(){
	
	APawn* OwnerPawn(Cast<APawn>(OwnerActor));
	// オーナーが正常なポーンクラスかチェック
	if((OwnerPawn != nullptr) && (OwnerPawn->Controller != nullptr)){
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(Cast<APlayerController>(OwnerPawn->Controller));
		// 入力周りの変更に関してはローカルコントローラに限定する
		if((PlayerController != nullptr) && (PlayerController->IsLocalController() == true)){
			// ゲームインスタンスを取得
			UGameInstance* GameInstance(SNUtility::GetGameInstance<UGameInstance>());
			// ゲームインスタンスが取得できない場合はアサート
			SNPLUGIN_ASSERT(GameInstance != nullptr, TEXT("GameInstance is nullptr"));
			// 入力マネージャーサブシステムを取得
			USNInputManagerSubsystem* InputManagerSubsystem(GameInstance->GetSubsystem<USNInputManagerSubsystem>());
			
			if(InputManagerSubsystem != nullptr){
				// マネージャーから解放
				InputManagerSubsystem->RemoveInputContext(GetConfigName());
			}
		}
	}
	// オーナーを初期化
	OwnerActor = nullptr;
	// アクションをリセット
	ActionList.Empty();
}
