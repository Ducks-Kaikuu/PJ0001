// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PJDamageAbilityTask.generated.h"

class UPJDamageGameplayEffect;
class UChooserTable;
/**
 * 
 */
UCLASS(Blueprintable)
class PJ0000_API UPJDamageAbilityTask : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, meta =(NotBlueprintThreadSafe))
	FGameplayEventData GetDamageTags();
	
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PJ|Damage")
	float DamageValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PJ|Damage")
	FGameplayTagContainer DamageAttributeTag;
	
private:

	UFUNCTION()
	void OnEndPlayMontage(FName NotifyName);

	UPROPERTY(EditAnywhere, Category = "PJ|Damage")
	TArray<TSubclassOf<UGameplayEffect>> EffectList;

	UPROPERTY(EditDefaultsOnly, Category = "PJ|Damage")
	TSubclassOf<UPJDamageGameplayEffect> DamageEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "PJ|Damage")
	TObjectPtr<UChooserTable> DamageAnimationChooser = nullptr;
};
