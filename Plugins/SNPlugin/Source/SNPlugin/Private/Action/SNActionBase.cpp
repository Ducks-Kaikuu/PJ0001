// Fill out your copyright notice in the Description page of Project Settings.
#include "Action/SNActionBase.h"
#include "EnhancedInputComponent.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Base/SNPlayablePawnInterface.h"
#include "Utility/SNUtility.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
FInputActionValueProxy::FInputActionValueProxy():
 Super()
{
	
}

//----------------------------------------------------------------------//
//
//! @brief コピーコンストラクタ
//
//! @param InputActionValue コピー元
//
//----------------------------------------------------------------------//
FInputActionValueProxy::FInputActionValueProxy(const FInputActionValue& InputActionValue):
 Super(InputActionValue.GetValueType(), Axis3D(InputActionValue[0], InputActionValue[1], InputActionValue[2]))
{
	
}

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNActionBase::USNActionBase():
 Super()
,Owner(nullptr)
{
	
}

//----------------------------------------------------------------------//
//
//! @brief デストラクタ
//
//----------------------------------------------------------------------//
USNActionBase::~USNActionBase(){
	Owner = nullptr;
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//! @param InputComponent インプットコンポーネント
//! @param InputAction    インプットアクション
//! @param Object         オーナーとなるオブジェクト
//
//----------------------------------------------------------------------//
void USNActionBase::Initialize(UEnhancedInputComponent* InputComponent, const UInputAction* InputAction, UObject* Object){
	
	Owner = Object;
	
	if(InputComponent != nullptr){
		InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &USNActionBase::InputAction);	
	} else {
		
		FString OwnerName = (Owner != nullptr) ? Owner->GetName() : FString("No Owner.");
		
		SNPLUGIN_LOG(TEXT("Action Initialize is failed. InputComponent is nullptr.[%s]"), *OwnerName);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 入力からのアクション
//
//! @param InputActionValue 入力情報
//
//----------------------------------------------------------------------//
void	USNActionBase::InputAction(const FInputActionValue& InputActionValue){

	ASNCharacterBase* Character = static_cast<ASNCharacterBase*>(Owner);

	bool bExclusive = Character->GetActionTags().HasAny(ExclusiveTags);

	if(bExclusive == true)
	{
		SNPLUGIN_LOG(TEXT("USNActionBase::InputAction Exclusive Tag Contained."));
		
		return;
	}

	bool Result = false;
	
	if(SNUtility::IsServer(GetWorld()) == true){

		Result = ExecAction(InputActionValue);

		if (Result == true)
		{
			if(bExecOnBoth == true){
			
				ISNPlayablePawnInterface* PlayablePawn(GetOwner<ISNPlayablePawnInterface>());
			
				if(PlayablePawn != nullptr){
					PlayablePawn->ExecuteActionOnMulticast(GetActionTag(), InputActionValue);
				}
			}

			Character->AddActionTag(GetActionTag());;
		}
	} else {
		
		if((bExecOnServer == true) || (bExecOnBoth == true)){
			
			ISNPlayablePawnInterface* PlayablePawn(GetOwner<ISNPlayablePawnInterface>());
			
			if(PlayablePawn != nullptr){
				PlayablePawn->ExecuteActionOnServer(GetActionTag(), InputActionValue);
			} else {
				// 実行処理
				Result = ExecAction(InputActionValue);
			}
			
		} else {
			// 実行処理
			Result = ExecAction(InputActionValue);
		}
	}

	if ((Result == true) && (Character->IsLocallyControlled() == true))
	{
		Character->AddActionTag(GetActionTag());;
	}
}
