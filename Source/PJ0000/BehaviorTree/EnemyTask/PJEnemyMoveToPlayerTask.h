// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PJEnemyMoveToPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyMoveToPlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float ChaseDistance=-1.0f;
};
