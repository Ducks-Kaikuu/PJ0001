// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/Ability/PJLaunchDamageAbility.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Channels/MovieSceneTimeWarpChannel.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "System/PJGameInstance.h"
#include "System/SNBlueprintFunctionLibrary.h"
#include "Utility/SNUtility.h"

class USNDamageWithChooserComponent;

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
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo->OwnerActor));

	if (Character != nullptr)
	{
		DamageAttributeTag = ActivationOwnedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"))));

		Character->SetActionTagContainer(DamageAttributeTag);
		
		USNDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<USNDamageWithChooserComponent>();

		UPlayMontageCallbackProxy* MontageProxy = DamageComponent->PlayDamageAnimation(DamageAttributeTag);
		
		if (MontageProxy != nullptr)
		{
			MontageProxy->OnCompleted.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
			MontageProxy->OnInterrupted.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
			MontageProxy->OnNotifyBegin.AddDynamic(this, &UPJLaunchDamageAbility::OnMontageNotify);
		}
	}
}

void UPJLaunchDamageAbility::OnMontageNotify(FName NotifyName)
{
	if (NotifyName == TEXT("EndAbility"))
	{
		FGameplayAbilityActorInfo ActorInfo(GetActorInfo());

		ASNCharacterBase* Character(Cast<ASNCharacterBase>(ActorInfo.OwnerActor));

		if ((Character != nullptr) && (DamageAttributeTag.IsValid() == true))
		{
			Character->RemoveActionTagContainer(DamageAttributeTag);

			DamageAttributeTag.Reset();
		}
		
		K2_EndAbility();	
	}
	
	SNPLUGIN_LOG(TEXT("UPJLaunchDamageAbility::OnMontageNotify : %s"), *NotifyName.ToString());
}
