// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/SNGameInstance.h"
#include "PJGameInstance.generated.h"

class UPJTimerManager;
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

	UPJTimerManager* GetTimerManager();

private:

	UPROPERTY(EditAnywhere, Category="Game|Enemy")
	TSoftClassPtr<UPJEnemyManager> EnemyManagerClass;
	
	UPROPERTY(EditAnywhere, Category="Game|Damage")
	TSoftObjectPtr<UDamageData> DamageDataObject;

	UPROPERTY(EditAnywhere, Category="Game|Timer")
	TSoftClassPtr<UPJTimerManager> TimerManagerClass;

	UPROPERTY()
	TObjectPtr<UDamageData> DamageData = nullptr;

	UPROPERTY()
	TObjectPtr<UPJEnemyManager> EnemyManager = nullptr;

	UPROPERTY()
	TObjectPtr<UPJTimerManager> TimerManager = nullptr;
};

FORCEINLINE UPJEnemyManager* UPJGameInstance::GetEnemyManager()
{
	return EnemyManager;
}

FORCEINLINE UPJTimerManager* UPJGameInstance::GetTimerManager()
{
	return TimerManager;
}