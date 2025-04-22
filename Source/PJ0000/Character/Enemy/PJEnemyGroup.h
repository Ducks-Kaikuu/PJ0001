// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/NoExportTypes.h"
#include "PJEnemyGroup.generated.h"

class APJEnemyBase;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyGroup : public UObject
{
	GENERATED_BODY()

public:

	void SpawnGroupsEnemy(const TSubclassOf<APJEnemyBase>& EnemyClass, int Num, const FVector& SpawnLocation, float SpawnRadius);

	static FGenericTeamId GetTeamId();

	static FGameplayTag GetEqsName(FGenericTeamId TeamId);

	void RemoveMember(APJEnemyBase* Member);
	
private:

	AActor* SpawnEnemy(const TSubclassOf<APJEnemyBase>& EnemyClass, const FVector& SpawnLocation, float SpawnRadius);

	void SetupGroup(APJEnemyBase* Enemy);

	FGenericTeamId TeamID = FGenericTeamId::NoTeam;

	UPROPERTY()
	TArray<TObjectPtr<APJEnemyBase>> Members; 
};
