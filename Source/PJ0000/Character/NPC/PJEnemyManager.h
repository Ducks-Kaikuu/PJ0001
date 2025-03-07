// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PJEnemyManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FEnemyGine)

class APJEnemy;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyManager : public UObject
{
	GENERATED_BODY()

public:

	void AddEnemy(APJEnemy* Enemy);

	void RemoveEnemy(APJEnemy* Enemy);

	FEnemyGine OnEnemyGone;
	
private:
	UPROPERTY()
	TArray<APJEnemy*> EnemyList; 
};
