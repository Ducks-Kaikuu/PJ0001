// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Abilities/PJDamageAbilityTask.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
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
//		SNPLUGIN_ERROR(TEXT("DataTable is null!"));

//		return;
	}

	//TArray<const FDamageTable*> DamageList(DamageData->GetDamageList(ActivationOwnedTags));

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo->OwnerActor));

	if (Character != nullptr)
	{
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

		CommitAbility(Handle, ActorInfo, ActivationInfo);
		
		USNAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();

		if (AbilitySystemComponent != nullptr)
		{
			UClass* Damage = nullptr;

			if (DamageEffect.IsNull() == false)
			{
				Damage = Cast<UClass>(DamageEffect.LoadSynchronous());
			}

			if (Damage)
			{
				SNPLUGIN_LOG(TEXT("Class is null."));
			}

			FGameplayEffectSpecHandle SpecHandle(nullptr);
			
			//if (Damage)
			{
				UGameplayEffect* GameplayEffect = Damage->GetDefaultObject<UGameplayEffect>();

				FGameplayEffectSpec* NewSpec = new FGameplayEffectSpec(GameplayEffect, AbilitySystemComponent->MakeEffectContext(), 1.0f);
				
				SpecHandle =  FGameplayEffectSpecHandle(NewSpec);
			}
			
			//FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Damage, 1.0f, AbilitySystemComponent->MakeEffectContext());

			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

			if (Spec != nullptr)
			{
				Spec->AddDynamicAssetTag(TAG_Gameplay_DamageSelfDestruct);
			}
		}
	}

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage is comming."));
}

void UPJDamageAbilityTask::OnEndPlayMontage(FName NotifyName)
{
	K2_EndAbility();
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("On End Play Montage."));
}
