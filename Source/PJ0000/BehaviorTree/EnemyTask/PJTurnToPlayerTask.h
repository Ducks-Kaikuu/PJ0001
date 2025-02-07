// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJTurnToPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJTurnToPlayerTask : public USNActionBase
{
	GENERATED_BODY()
public:

	virtual bool ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere)
	float LimitAngle = 30.0f;
};
