// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Abilities/Ability/PJGameplayAbility.h"
#include "PJLaunchDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJLaunchDamageAbility : public UPJGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageNotify(FName NotifyName);
};
