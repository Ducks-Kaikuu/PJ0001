// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/SNPlayablePawnInterface.h"

#include "SNDef.h"
#include "Action/SNActionBase.h"
#include "Utility/SNUtility.h"

// Add default functionality here for any ISNPlayablePawnInterface functions that are not pure virtual.

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
ISNPlayablePawnInterface::ISNPlayablePawnInterface()
{
	
}

//----------------------------------------------------------------------//
//
//! @brief アクションを実行
//
//! @param Name             アクション名
//! @param InputActionValue 入力値
//
//----------------------------------------------------------------------//
void ISNPlayablePawnInterface::ExecuteAction(const FGameplayTag& Tag, const FInputActionValue& InputActionValue){
	
	USNActionBase* Action(GetAction(Tag));
	
	if(Action == nullptr){
		
		SNPLUGIN_LOG(TEXT("Action is nullptr."));
		
		return;
	}

	// 入力値を出力
	//SNPLUGIN_LOG(TEXT("Input Value : %f : %f"), InputActionValue[0], InputActionValue[1]);
	// 入力情報を出力
	SNPLUGIN_LOG(TEXT("[%s] - [%s]Input Action is calling. %f : %f"), *Action->GetOwner<AActor>()->GetName(), *(Action->GetName()), InputActionValue[0], InputActionValue[1]);
	// アクションを実行
	Action->ExecAction(InputActionValue);
	
	if(Action->IsExecOnBoth() == true){
		ExecuteActionOnMulticast(Tag, InputActionValue);
	}
}

//----------------------------------------------------------------------//
//
//! @brief サーバー側でアクションを実行
//
//! @param Name             アクション名
//! @param InputActionValue 入力情報
//
//----------------------------------------------------------------------//
void ISNPlayablePawnInterface::ExecuteActionOnServer(const FGameplayTag& Tag, const FInputActionValue& InputActionValue){
	// サーバーの場合は処理を回さない
	if(SNUtility::IsServer(GetPrimaryWorld()) != true){
		
		FInputActionValueProxy Proxy(InputActionValue);
		
		ExecuteInputAction_OnServer(Tag, Proxy.GetValue(), Proxy.GetValueType());
	}
}

//----------------------------------------------------------------------//
//
//! @brief クライアント側でアクションを実行
//
//! @param Name             アクション名
//! @param InputActionValue 入力情報
//
//----------------------------------------------------------------------//
void ISNPlayablePawnInterface::ExecuteActionOnMulticast(const FGameplayTag& Tag, const FInputActionValue& InputActionValue){
	// クライアントの場合は処理を回さない
	if(SNUtility::IsServer(GetPrimaryWorld()) == true){
		
		FInputActionValueProxy Proxy(InputActionValue);
		
		SNPLUGIN_LOG(TEXT("Excute Action Client is called."));
		
		ExecuteInputAction_OnMulticast(Tag, Proxy.GetValue(), Proxy.GetValueType());
	}
}

//----------------------------------------------------------------------//
//
//! @brief レプリケーションからアクション実行
//
//! @param Name       アクション名
//! @param InputValue 入力情報
//! @param Type       入力タイプ
//
//----------------------------------------------------------------------//
void ISNPlayablePawnInterface::ExexuteInputActionImplement(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type){
	// 入力情報を再構築
	FInputActionValue InputActionValue(Type, InputValue);
	// アクションを実行
	ExecuteAction(Tag, InputActionValue);
}
