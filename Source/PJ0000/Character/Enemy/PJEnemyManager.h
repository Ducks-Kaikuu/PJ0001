// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PJEnemyManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FEnemyGine)

class APJEnemyBase;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyManager : public UObject
{
	GENERATED_BODY()

public:

	void AddEnemy(APJEnemyBase* Enemy);

	void RemoveEnemy(APJEnemyBase* Enemy);

	FEnemyGine OnEnemyGone;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<APJEnemyBase>> EnemyList; 
};
