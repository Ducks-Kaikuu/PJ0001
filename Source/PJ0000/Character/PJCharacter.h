// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "PJCharacter.generated.h"

class UPJHealthSet;
class UGameplayAbility;
class AActor;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS()
class PJ0000_API APJCharacter : public ASNMotionMatchingPlayerBase
{
	GENERATED_BODY()

public:
	APJCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
private:

	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

};
