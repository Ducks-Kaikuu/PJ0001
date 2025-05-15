// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/PJEnemyBase.h"

#include "PJEnemyGroup.h"
#include "PJEnemyManager.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Character/PJCharacterDef.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Components/SNMaterialComponent.h"
#include "System/PJGameInstance.h"
#include "System/SNBlueprintFunctionLibrary.h"
#include "Utility/SNUtility.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//! @param ObjectInitializer イニシャライザ
//
//----------------------------------------------------------------------//
APJEnemyBase::APJEnemyBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	
}

const APJAIEnemy000* APJEnemyBase::GetAIController() const
{
	return Cast<APJAIEnemy000>(GetController());
}

APJAIEnemy000* APJEnemyBase::GetAIController()
{
	return Cast<APJAIEnemy000>(GetController());
}

void APJEnemyBase::SetTeamID(const FGenericTeamId& TeamID)
{
	APJAIEnemy000* AIController = GetAIController();
	
	if(AIController != nullptr){
		AIController->SetGenericTeamId(TeamID);
	}
}

FGenericTeamId APJEnemyBase::GetTeamID() const
{
	const APJAIEnemy000* AIController = GetAIController();

	if(AIController != nullptr){
		return AIController->GetGenericTeamId();
	}
	
	return FGenericTeamId::NoTeam;
}

void APJEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	APJAIEnemy000* AIController = GetAIController();
	
	if(AIController != nullptr)
	{
		// 初期値を設定。
		AIController->SetGenericTeamId(static_cast<uint8>(ETeamID::Enemy_AdJoin));
		
		APlayerController* PlayerController = Cast<APlayerController>(SNUtility::GetPlayerController<APlayerController>());
		
		if(PlayerController != nullptr){
			AIController->SetPlayerKey(PlayerController->GetPawn());
		}
	}
	
	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();
	
	if (GameInstance != nullptr)
	{
		GameInstance->GetEnemyManager()->AddEnemy(this);
	}
	
	if(USNMaterialComponent* MaterialComponent = FindComponentByClass<USNMaterialComponent>())
	{
		MaterialComponent->CreateMaterialInstanceDynamic();
	}
}

void APJEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance != nullptr)
	{
		GameInstance->GetEnemyManager()->RemoveEnemy(this);
	}

	if (GroupManager != nullptr)
	{
		GroupManager->RemoveMember(this);

		GroupManager = nullptr;
	}
}

void APJEnemyBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (OnLanded.IsBound() == true)
	{
		OnLanded.Execute(Hit);
	}
}

bool APJEnemyBase::IsDead() const
{
	return IPJFighterInterface::IsDead();
#if 0
	const UPJHealthSet* HealthSet = GetGameAttribute<UPJHealthSet>();
	
	if (HealthSet != nullptr)
	{
		int HP = HealthSet->GetHealth();

		return (HP <= 0) ? true : false;
	}

	return false;
#endif
}

UPlayMontageCallbackProxy* APJEnemyBase::PlayAnimMontageByActionTag()
{
	if (ChooserTable == nullptr)
	{
		return nullptr;
	}

	float PlayRate = 1.0f;
	float StartTime = 0.0f;
	
	UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(this, ChooserTable, this, PlayRate, StartTime);

	if (AnimMontage != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			if (AnimInstance->Montage_IsPlaying(AnimMontage) == false)
			{
				MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), AnimMontage, 1.0f, 0.0f);
				
				SNPLUGIN_LOG(TEXT("Motion is %s"), *AnimMontage->GetName());
			}
		}
		
		return MontageProxy;
	}

	return nullptr;
}

void APJEnemyBase::SetAttackMotionDelegate(UPlayMontageCallbackProxy* Proxy, const FGameplayTag& Tag, FOnMontagePlayDelegate& Delegate)
{
}

void APJEnemyBase::SetAttackMotionDelegate(UPlayMontageCallbackProxy* Proxy, const FGameplayTag& Tag)
{
	if (Proxy != nullptr)
	{
		if (Proxy->OnCompleted.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnCompleted.AddDynamic(this, &APJEnemyBase::OnAttackMotionEndplayMontage);
		}

		if (Proxy->OnInterrupted.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnInterrupted.AddDynamic(this, &APJEnemyBase::OnAttackMotionEndplayMontage);
		}
					
		if (Proxy->OnBlendOut.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnBlendOut.AddDynamic(this, &APJEnemyBase::OnAttackMotionEndplayMontage);
		}
	}
	
	AttackTag = Tag;
}

void APJEnemyBase::OnAttackMotionEndplayMontage(FName NotifyName)
{
	if (AttackTag.IsValid() == true)
	{
		RemoveActionTag(AttackTag);
	}

	ResetMontagePlayProxy();
}

