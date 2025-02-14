// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Abilities/Ability//PJDamageAbilityTask.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
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

		TArray<const FDamageTable*> DamageList(DamageData->GetDamageList(ActivationOwnedTags));
		// ダメージの属性は1個だけ...。ダメージの持続時間が複数個に対応できるなら...複数個にしても大丈夫ですが...。
		SNPLUGIN_ASSERT(DamageList.Num() ==1, TEXT("Invalidate damage attributes."));

		const FDamageTable* Damage = DamageList[0];
		
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
		}
		
		UPJDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<UPJDamageWithChooserComponent>();
		
		if (DamageComponent != nullptr)
		{
			DamageComponent->DrawDamage(Damage->Damage);

			UPlayMontageCallbackProxy* MontageProxy = DamageComponent->PlayDamageAnimation(DamageAttributeTag);

			if (MontageProxy != nullptr)
			{
				MontageProxy->OnCompleted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnInterrupted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnBlendOut.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnNotifyBegin.AddDynamic(this, &UPJDamageAbilityTask::OnNotifyBegin);

				return;
			}
		}
	}
}

void UPJDamageAbilityTask::OnNotifyBegin(FName NotifyName)
{
	if (NotifyName == FName(TEXT("EndDamage")))
	{
		FGameplayAbilityActorInfo ActorInfo(GetActorInfo());

		ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo.OwnerActor));

		if (Character != nullptr)
		{
			Character->RemoveActionTagContainer(DamageAttributeTag);

			DamageAttributeTag.Reset();
			
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On Notify Begin."));
		}
	}
	else
	{
		
	}
}

void UPJDamageAbilityTask::OnEndPlayMontage(FName NotifyName)
{
	FGameplayAbilityActorInfo ActorInfo(GetActorInfo());

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo.OwnerActor));

	if (Character != nullptr)
	{
		if (DamageAttributeTag.IsValid() == true)
		{
			Character->RemoveActionTagContainer(DamageAttributeTag);

			DamageAttributeTag.Reset();
		}
		
		Character->RemoveActionTag(DamageState);
	}
	
	K2_EndAbility();
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On End Play Montage."));
}
