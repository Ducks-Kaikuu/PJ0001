// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "SNGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	void ApplyGameplayEffectToTarget(UAbilitySystemComponent* SelfAbilitySystemComponent, UAbilitySystemComponent* TargetAbilitySystemComponent);

	void ApplyGameplayEffectToTarget(UAbilitySystemComponent* SelfAbilitySystemComponent, UAbilitySystemComponent* TargetAbilitySystemComponent, TFunctionRef<void(FGameplayEffectSpec*)> EffectPredicate);
	
	void ApplyGameplayEffectsToSelf(UAbilitySystemComponent* AbilitySystemComponent);
	
	void ApplyGameplayEffectsToSelf(UAbilitySystemComponent* AbilitySystemComponent, TFunctionRef<void(FGameplayEffectSpec*)> EffectPredicate);
	
private:

	UPROPERTY(EditAnywhere, Category = "SN|Abilities|Effect")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;
	
};
