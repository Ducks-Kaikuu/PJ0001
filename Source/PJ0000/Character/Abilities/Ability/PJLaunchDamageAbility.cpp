// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Ability/PJLaunchDamageAbility.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Channels/MovieSceneTimeWarpChannel.h"
#include "Character/Base/SNCharacterBase.h"
#include "System/PJGameInstance.h"
#include "System/SNBlueprintFunctionLibrary.h"
#include "Utility/SNUtility.h"

void UPJLaunchDamageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	DamageAttributeTag = ActivationOwnedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"))));
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo->OwnerActor));

	if (Character != nullptr)
	{
		float PlayRate = 1.0f;
		float StartTime= 0.0f;
		
		UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(Character, DamageAnimationChooser, this, PlayRate, StartTime);

		if (AnimMontage != nullptr)
		{
			UPlayMontageCallbackProxy* MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(Character->GetMesh(), const_cast<UAnimMontage*>(AnimMontage), 1.0f, 0.0f, NAME_None);

			if (MontageProxy != nullptr)
			{
				MontageProxy->OnCompleted.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
				MontageProxy->OnInterrupted.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
				MontageProxy->OnNotifyBegin.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
			}
		}
	}
	
}

void UPJLaunchDamageAbility::OnMontageNotify(FName NotifyName)
{
	if (NotifyName == TEXT("EndAbility"))
	{
		K2_EndAbility();	
	}
	
	SNPLUGIN_LOG(TEXT("UPJLaunchDamageAbility::OnMontageNotify : %s"), *NotifyName.ToString());
}
