// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Abilities/PJDamageAbilityTask.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PJ0000/Character/NPC/PJEnemy.h"
#include "PJ0000/Damage/PJDamageData.h"
#include "PJ0000/Damage/PJDamageGameplayEffect.h"
#include "PJ0000/System/PJGameInstance.h"
#include "System/SNBlueprintFunctionLibrary.h"
#include "Utility/SNUtility.h"

FGameplayEventData UPJDamageAbilityTask::GetDamageTags()
{
	FGameplayEventData DamageTags;

	DamageTags.TargetTags = ActivationOwnedTags;
	
	return DamageTags;
}

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

	DamageAttributeTag = ActivationOwnedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"))));

	if (Character != nullptr)
	{
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
		
		float PlayRate = 1.0f;
		float StartTime = 0.0f;
		
		UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(Character, DamageAnimationChooser, this, PlayRate, StartTime);

		if (AnimMontage != nullptr)
		{
			UPlayMontageCallbackProxy* MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(Character->GetMesh(), const_cast<UAnimMontage*>(AnimMontage), 1.0f, 0.0f, NAME_None);

			if (MontageProxy != nullptr)
			{
				MontageProxy->OnCompleted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnInterrupted.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
				MontageProxy->OnBlendOut.AddDynamic(this, &UPJDamageAbilityTask::OnEndPlayMontage);
			}
		}

		APJEnemy* Enemy = Cast<APJEnemy>(Character);

		if (Enemy != nullptr)
		{
			Enemy->DrawDamage(22);
		}
	}

	//CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage is comming."));
}

void UPJDamageAbilityTask::OnEndPlayMontage(FName NotifyName)
{
	K2_EndAbility();
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On End Play Montage."));
}
