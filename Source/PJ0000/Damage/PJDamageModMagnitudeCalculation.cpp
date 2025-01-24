// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Damage/PJDamageModMagnitudeCalculation.h"

#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"

UPJDamageModMagnitudeCalculation::UPJDamageModMagnitudeCalculation()
{
	MaxHealthDef.AttributeToCapture = UPJHealthSet::GetMaxHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthDef.bSnapshot = false;

	HealthDef.AttributeToCapture = UPJHealthSet::GetHealthAttribute();
	HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxHealthDef);
	RelevantAttributesToCapture.Add(HealthDef);
}

float UPJDamageModMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Magnitude = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	return Magnitude + 100000.0f; 
}
