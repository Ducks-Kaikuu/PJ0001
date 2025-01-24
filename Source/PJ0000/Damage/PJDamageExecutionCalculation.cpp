// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Damage/PJDamageExecutionCalculation.h"

#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"

struct FHealthStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseHealthDef;
	
	FHealthStatics()
	{
		BaseHealthDef = FGameplayEffectAttributeCaptureDefinition(UPJHealthSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FHealthStatics& HealthStatics()
{
	static FHealthStatics HealthStatics;

	return HealthStatics;
}

UPJDamageExecutionCalculation::UPJDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(HealthStatics().BaseHealthDef);;
}

void UPJDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPJHealthSet::GetHealthAttribute(), EGameplayModOp::Additive, 10.0f));
}
