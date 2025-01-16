// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/SNGameInstance.h"
#include "PJGameInstance.generated.h"

class UDamageData;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJGameInstance : public USNGameInstance
{
	GENERATED_BODY()

public:
	
	virtual void Init() override;

	UDamageData* GetDamageData();

private:
	
	UPROPERTY(EditAnywhere, Category="Game|Damage")
	TSoftObjectPtr<UDamageData> DamageDataObject;

	UPROPERTY()
	TObjectPtr<UDamageData> DamageData = nullptr;
};
