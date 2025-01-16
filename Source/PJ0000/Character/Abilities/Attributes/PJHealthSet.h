// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "Character/AbilitySystem/SNAttributeSet.h"
#include "PJHealthSet.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth)
/**
 * 
 */
UCLASS(Blueprintable)
class PJ0000_API UPJHealthSet : public USNAttributeSet
{
	GENERATED_BODY()

public:

	UPJHealthSet();
	
	ATTRIBUTE_ACCESSORS(UPJHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UPJHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPJHealthSet, Damage);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	mutable FSNAttributeEvent OnHealthChanged;

	mutable FSNAttributeEvent OnMaxHealthChanged;
	
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;	

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const ;
	
private:

	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Health, Category="Health")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_MaxHealth, Category="Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, Category="Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
