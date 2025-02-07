// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJEnemyAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyAttackTask : public USNActionBase
{
	GENERATED_BODY()

protected:
	
	virtual bool ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UFUNCTION()
	void OnEndplayMontage(FName NotifyName);
	
	UPROPERTY(EditAnywhere)
	float AttackRagen=100.0f;
};
