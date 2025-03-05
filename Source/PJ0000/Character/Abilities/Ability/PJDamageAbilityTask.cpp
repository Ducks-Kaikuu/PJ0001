// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Abilities/Ability//PJDamageAbilityTask.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "Character/NPC/PJEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PJ0000/Damage/PJDamageData.h"
#include "PJ0000/System/PJGameInstance.h"
#include "Utility/SNUtility.h"

void UPJDamageAbilityTask::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("Gameplay Instance is null!"));

		return;
	}

	UDamageData* DamageData = GameInstance->GetDamageData();

	if (DamageData == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("DataTable is null!"));

		return;
	}

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo->OwnerActor));

	if (Character != nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage is comming."));
		
		DamageAttributeTag = ActivationOwnedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"))));
		
		Character->SetActionTagContainer(DamageAttributeTag);
		
		if (DamageState.IsValid())
		{
			Character->AddActionTag(DamageState);
		}

		UPJDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<UPJDamageWithChooserComponent>();

		TArray<const FDamageTable*> DamageList(DamageData->GetDamageList(ActivationOwnedTags));
		// ダメージの属性は1個だけ...。ダメージの持続時間が複数個に対応できるなら...複数個にしても大丈夫ですが...。
		SNPLUGIN_ASSERT(DamageList.Num() ==1, TEXT("Invalidate damage attributes."));
		
		const FDamageTable* Damage = DamageList[0];

		SNPLUGIN_ASSERT(Damage != nullptr, TEXT("Damage is null!"));

		if (Damage->Duration > 0.0f)
		{
			Character->GetWorldTimerManager().SetTimer(DamageTimers, this, &UPJDamageAbilityTask::OnDamageFinished, Damage->Duration, false);

			SNPLUGIN_LOG(TEXT("Damage Timer Started."));
		} else
		{
			if(Character->GetCharacterMovement()->IsMovingOnGround() == true)
			{
				Character->GetWorldTimerManager().SetTimer(DamageTimers, this, &UPJDamageAbilityTask::OnDamageFinished, 0.1f, false);
			}
		}
		
		if (DamageComponent != nullptr)
		{
			UAnimInstance* AnimInstance = Character->GetAnimInstance();

			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Resume(nullptr);
			}
				
			DamageComponent->AddAirDamageTimer(-1.0f);
				
			DamageComponent->SetDamageAbilityTags(DamageAttributeTag);
		}
		
		USNAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
		
		if (AbilitySystemComponent != nullptr)
		{
			ApplyGameplayEffectsToSelf(AbilitySystemComponent, [&](FGameplayEffectSpec* Spec)
			{
				if (Spec != nullptr)
				{
					FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"));

					Spec->SetSetByCallerMagnitude(DamageTag, Damage->Damage);
				}
			});
			
			if(DamageComponent != nullptr)
			{
				const FGameplayEffectContextHandle& ContextHandle(DamageComponent->GetDamagedEffectContextHandle());

				for (auto& GameplayTag : Damage->DamageCueTags)
				{
					AbilitySystemComponent->ExecuteGameplayCue(GameplayTag, ContextHandle);	
				}

				DamageComponent->ResetDamagedEffectContextHandle();
			}
		}
		
		if (DamageComponent != nullptr)
		{
			// PIEウィンドウからフォーカスを移すと高確率でハングするので、とりあえずこめんと
			// DamageComponent->DrawDamage(Damage->Damage);
			// 地面にいる状態ならダメージモーションを再生(空中でのダメージはモーションがないの...)
			if ((Character->GetCharacterMovement()->IsMovingOnGround() == true)
			||  (DamageAttributeTag.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.Strike")))))
			{
				UPlayMontageCallbackProxy* MontageProxy(DamageComponent->PlayDamageAnimation(DamageAttributeTag));

				if (MontageProxy != nullptr)
				{
					MontageProxy->OnNotifyBegin.AddDynamic(this, &UPJDamageAbilityTask::OnNotifyBegin);
				}
			}

			if (DamageAttributeTag.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.Strike"))))
			{
				DamageComponent->ResetAirDamageTimer();
			}
		}
	}
}

void UPJDamageAbilityTask::OnNotifyBegin(FName NotifyName)
{
	if (NotifyName == FName(TEXT("EndDamage")))
	{
		OnDamageFinished();
	}
}

void UPJDamageAbilityTask::OnDamageFinished()
{
	FGameplayAbilityActorInfo ActorInfo(GetActorInfo());
	
	APJEnemy* Character(Cast<APJEnemy>(ActorInfo.OwnerActor));

	if (Character != nullptr)
	{
		if (Character->IsDead())
		{
			return;
		}
		
		if (DamageAttributeTag.IsValid() == true)
		{
			Character->RemoveActionTagContainer(DamageAttributeTag);

			DamageAttributeTag.Reset();
		}
	}

	DamageTimers.Invalidate();

	K2_EndAbility();

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On Damage Finished."));
}
