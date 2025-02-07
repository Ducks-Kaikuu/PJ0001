// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "PJDamageWithChooserComponent.generated.h"

class USNUserWidgetBase;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ0000_API UPJDamageWithChooserComponent : public USNDamageWithChooserComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Damage")
	void DrawDamage(int Damage);

private:
	
	UPROPERTY(EditAnywhere, Category="Damage")
	TSubclassOf<USNUserWidgetBase> DamageWidget = nullptr;

};
