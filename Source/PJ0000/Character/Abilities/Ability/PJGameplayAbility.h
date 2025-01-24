// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AbilitySystem/SNGameplayAbility.h"
#include "PJGameplayAbility.generated.h"

class UChooserTable;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJGameplayAbility : public USNGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PJ|Damage")
	FGameplayTagContainer DamageAttributeTag;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PJ|Damage")
	TObjectPtr<UChooserTable> DamageAnimationChooser = nullptr;
};
