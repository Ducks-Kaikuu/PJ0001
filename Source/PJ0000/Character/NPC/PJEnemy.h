// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/SNCharacterBase.h"
#include "PJEnemy.generated.h"

struct FGameplayEffectSpec;

class UPJHealthSet;

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PJ0000_API APJEnemy : public ASNCharacterBase
{
	GENERATED_BODY()
public:
	APJEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	
private:
	
	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UPROPERTY()
	TObjectPtr<UPJHealthSet> HealthSet;	
};
