// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/SNCheatManager.h"
#include "PJCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJCheatManager : public USNCheatManager
{
	GENERATED_BODY()

public:

	virtual void InitCheatManager() override;
	
	UFUNCTION(exec, meta = (Exec))
	void PJEnemyNoDamage(bool Flag);

	bool IsEnemyNoDamage() const { return bNoDamage; }
	
private:
	
	bool bNoDamage = false;
};
