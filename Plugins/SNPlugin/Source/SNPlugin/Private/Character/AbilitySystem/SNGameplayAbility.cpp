// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilitySystem/SNGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Online/OnlineErrorCode.h"

void USNGameplayAbility::ApplyGameplayEffectToTarget(UAbilitySystemComponent* SelfAbilitySystemComponent, UAbilitySystemComponent* TargetAbilitySystemComponent)
{
	ApplyGameplayEffectToTarget(SelfAbilitySystemComponent, TargetAbilitySystemComponent, [](FGameplayEffectSpec*){});
}

void USNGameplayAbility::ApplyGameplayEffectToTarget(UAbilitySystemComponent* SelfAbilitySystemComponent, UAbilitySystemComponent* TargetAbilitySystemComponent, TFunctionRef<void(FGameplayEffectSpec*)> EffectPredicate)
{
	for (auto& Effect: GameplayEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = SelfAbilitySystemComponent->MakeOutgoingSpec(Effect, 1.0f, SelfAbilitySystemComponent->MakeEffectContext());

		
		if (SpecHandle.Data != nullptr)
		{
			EffectPredicate(SpecHandle.Data.Get());
			
			SelfAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComponent);
		}
	}
}

void USNGameplayAbility::ApplyGameplayEffectsToSelf(UAbilitySystemComponent* AbilitySystemComponent)
{
	ApplyGameplayEffectsToSelf(AbilitySystemComponent, [](FGameplayEffectSpec*){});
}

void USNGameplayAbility::ApplyGameplayEffectsToSelf(UAbilitySystemComponent* AbilitySystemComponent, TFunctionRef<void(FGameplayEffectSpec*)> EffectPredicate)
{
	for (auto& Effect : GameplayEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.0f, AbilitySystemComponent->MakeEffectContext());

		if (SpecHandle.Data != nullptr)
		{
			EffectPredicate(SpecHandle.Data.Get());
			
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}
}
