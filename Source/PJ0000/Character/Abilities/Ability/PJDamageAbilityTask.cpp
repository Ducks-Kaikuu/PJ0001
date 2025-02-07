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
#include "PJ0000/Character/NPC/PJEnemy.h"
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

	TArray<const FDamageTable*> DamageList(DamageData->GetDamageList(ActivationOwnedTags));

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
				
		USNAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();

		if (AbilitySystemComponent != nullptr)
		{
			ApplyGameplayEffectsToSelf(AbilitySystemComponent, [&](FGameplayEffectSpec* Spec)
			{
				if (Spec != nullptr)
				{
					FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"));

					Spec->SetSetByCallerMagnitude(DamageTag, 22.0f);
				}
			});
		}
		
		UPJDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<UPJDamageWithChooserComponent>();
		
		if (DamageComponent != nullptr)
		{
			DamageComponent->DrawDamage(22);
			
			UPlayMontageCallbackProxy* MontageProxy = DamageComponent->PlayDamageAnimation(DamageAttributeTag);
			
			if (MontageProxy != nullptr)
			{
				MontageProxy->OnCompleted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnInterrupted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnBlendOut.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);

				return;
			}
		}
	}
}

void UPJDamageAbilityTask::OnEndPlayMontage(FName NotifyName)
{
	FGameplayAbilityActorInfo ActorInfo(GetActorInfo());

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo.OwnerActor));

	if ((Character != nullptr) && (DamageAttributeTag.IsValid() == true))
	{
		Character->RemoveActionTagContainer(DamageAttributeTag);

		DamageAttributeTag.Reset();
	}
	
	K2_EndAbility();
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On End Play Montage."));
}
