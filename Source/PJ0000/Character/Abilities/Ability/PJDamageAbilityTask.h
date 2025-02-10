// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character//Abilities/Ability/PJGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PJDamageAbilityTask.generated.h"

class UPJDamageGameplayEffect;
class UChooserTable;
/**
 * 
 */
UCLASS(Blueprintable)
class PJ0000_API UPJDamageAbilityTask : public UPJGameplayAbility
{
	GENERATED_BODY()

public:

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PJ|Damage")
	FGameplayTagContainer DamageAttributeTag;
	
private:

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName);

	UFUNCTION()
	void OnEndPlayMontage(FName NotifyName);

	UPROPERTY(EditAnywhere, Category="PJ|Damage")
	FGameplayTag DamageState;

	UPROPERTY()
	FTimerHandle DamageTimers;
};
