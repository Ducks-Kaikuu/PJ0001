// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PJTimerManager.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJTimerManager : public UObject
{
	GENERATED_BODY()

public:

	void UpdateTimer();

	void SetWorldTimerRate(float Rate);

	void SetPlayerSpeedupRate(float Rate);

	void SetIgnoreTimeRate(AActor* Actor, bool bIgnore);

	void SetNiagaraIgnoreTimeRate(UNiagaraComponent* Component, bool bIgnore);

	float GetWorldTimerRate() const ;

	float GetPlayerSpeedupRate() const ;

	float GetPlayerTimerRate() const ;

	void AddIgnoreTimeRareVfx(UNiagaraComponent* Component);

private:
	
	UFUNCTION()
	void NiagaraSystemFinished(class UNiagaraComponent* Component);

	float WorldTimerRate = 1.0f;

	float PlayerTimerRate = 1.0f;

	float PlayerSpeedupRate = 1.0f;

	UPROPERTY()
	TArray<TObjectPtr<UNiagaraComponent>> IgnoreTimeRateVFX;
};

FORCEINLINE float UPJTimerManager::GetWorldTimerRate() const
{
	return WorldTimerRate;
}

FORCEINLINE float UPJTimerManager::GetPlayerSpeedupRate() const
{
	return PlayerSpeedupRate;
}

FORCEINLINE float UPJTimerManager::GetPlayerTimerRate() const
{
	return PlayerTimerRate;
} 