// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PJEnemyMoveToPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyMoveToPlayerTask : public USNActionBase
{
	GENERATED_BODY()

public:

	virtual bool ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	bool bDebugDraw = false;
#endif
	
	UPROPERTY(EditAnywhere)
	FGameplayTag IdleTag;
	
	UPROPERTY(EditAnywhere)
	float ChaseDistance=-1.0f;
};
