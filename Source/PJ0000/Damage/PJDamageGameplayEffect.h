// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PJDamageGameplayEffect.generated.h"

class UPJGameplayEffectComponent;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJDamageGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;

	virtual void PostLoad() override;

};
