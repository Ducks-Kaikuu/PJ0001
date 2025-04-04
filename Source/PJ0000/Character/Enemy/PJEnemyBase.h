// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/SNCharacterBase.h"
#include "PJEnemyBase.generated.h"

class UPJEnemyGroup;
class APJAIEnemy000;
struct FGenericTeamId;

/**
 * 
 */
UCLASS()
class PJ0000_API APJEnemyBase : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	
	APJEnemyBase(const FObjectInitializer& ObjectInitializer);
	
	APJAIEnemy000* GetAIController();
	const APJAIEnemy000* GetAIController() const ;

	void SetGroupManager(UPJEnemyGroup* Manager);

	void SetTeamID(const FGenericTeamId& TeamID);

	FGenericTeamId GetTeamID() const ;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	TObjectPtr<UPJEnemyGroup> GroupManager = nullptr;
};

FORCEINLINE void APJEnemyBase::SetGroupManager(UPJEnemyGroup* Manager)
{
	GroupManager = Manager;
}
