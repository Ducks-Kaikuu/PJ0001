// Fill out your copyright notice in the Description page of Project Settings.
#include "Action/SNActionBase.h"

#include "AIController.h"
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
	
	SNPLUGIN_ASSERT(Character != nullptr, TEXT("ActionBase : Character is nullptr."));
	
	bool bExclusive = Character->GetActionTags().HasAny(ExclusiveTags);
	
	if(bExclusive == true){
		
		SNPLUGIN_LOG(TEXT("USNActionBase::InputAction Exclusive Tag Contained."));
		
		return;
	}
	
	bool Result = false;
	
	if(SNUtility::IsServer(GetWorld()) == true){
		// アクション実行
		Result = ExecAction(InputActionValue);
		
		if(Result == true){
			// クライアントでも実行する場合
			if(bExecOnMulticast == true){
				// インターフェイスを取得
				ISNPlayablePawnInterface* PlayablePawn(GetOwner<ISNPlayablePawnInterface>());
				
				if(PlayablePawn != nullptr){
					// クライアントへマルチキャスト
					PlayablePawn->ExecuteActionOnMulticast(GetActionTag(), InputActionValue);
				}
			}
		}
	} else {
		
		if((bExecOnServer == true) || (bExecOnMulticast == true)){
			
			ISNPlayablePawnInterface* PlayablePawn(GetOwner<ISNPlayablePawnInterface>());
			
			if(PlayablePawn != nullptr){
				// サーバー側に実行をリクエスト
				PlayablePawn->ExecuteActionOnServer(GetActionTag(), InputActionValue);
			} else {
				// 実行処理
				Result = ExecAction(InputActionValue);
				// ワーニングを出力
				SNPLUGIN_WARNING(TEXT("USNActionBase : Request OnServer or OnMulticast, But ISNPlayablePawnInterface is nullptr."));
			}
		} else {
			// 実行処理
			Result = ExecAction(InputActionValue);
		}
	}
	// 実行に成功し、ローカル制御の場合のみアクションタグを追加
	if((Result == true) && (Character->IsLocallyControlled() == true) && (GetActionTag().IsValid())){
		Character->AddActionTag(GetActionTag());
	}
}

EBTNodeResult::Type USNActionBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* Controller(Cast<AAIController>(OwnerComp.GetAIOwner()));
	
	if(Controller != nullptr){
		// オーナーとなるキャラクターを取得
		ASNCharacterBase* Character(Cast<ASNCharacterBase>(Controller->GetPawn()));
		
		if(Character != nullptr){
			// 排他チェック
			bool bExclusive = Character->GetActionTags().HasAny(ExclusiveTags);
			
			if (bExclusive == false)
			{
				bool bSuccess = ExecAIAction(OwnerComp, NodeMemory);
				
				Result = (bSuccess == true) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

				if (GetActionTag().IsValid() == true)
				{
					Character->AddActionTag(GetActionTag());	
				}
			} else {
				Result = EBTNodeResult::Type::Succeeded;
			}
		}
	}
	
	return Result;
}
