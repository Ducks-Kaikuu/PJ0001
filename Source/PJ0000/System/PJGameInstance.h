// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/SNGameInstance.h"
#include "PJGameInstance.generated.h"

class UPJEnemyManager;
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

	UPJEnemyManager* GetEnemyManager();

private:

	UPROPERTY(EditAnywhere, Category="Game|Enemy")
	TSoftClassPtr<UPJEnemyManager> EnemyManagerClass;
	
	UPROPERTY(EditAnywhere, Category="Game|Damage")
	TSoftObjectPtr<UDamageData> DamageDataObject;

	UPROPERTY()
	TObjectPtr<UDamageData> DamageData = nullptr;

	UPROPERTY()
	TObjectPtr<UPJEnemyManager> EnemyManager = nullptr;
};

FORCEINLINE UPJEnemyManager* UPJGameInstance::GetEnemyManager()
{
	return EnemyManager;
}
