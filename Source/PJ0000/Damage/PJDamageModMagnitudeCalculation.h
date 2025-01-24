// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayModMagnitudeCalculation.h"
#include "PJDamageModMagnitudeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJDamageModMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UPJDamageModMagnitudeCalculation();

	UFUNCTION()
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	UPROPERTY()
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	
	UPROPERTY()
	FGameplayEffectAttributeCaptureDefinition HealthDef;
};
