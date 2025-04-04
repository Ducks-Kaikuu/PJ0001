// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "BehaviorTree/Tasks/BTTask_RunEQSQuery.h"
#include "PJRunEQSTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJRunEQSTask : public UBTTask_RunEQSQuery
{
	GENERATED_BODY()

public:
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	void OnPJQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult);
	
	UPROPERTY(EditAnywhere, Category = "SN|Debug")
	bool bDebugDraw = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Score|Remap")
	float MinThreshold = 1.0f;

	UPROPERTY(EditAnywhere, Category = "SN|Score|Remap")
	float MaxThreshold = 2.0f;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerBTComp = nullptr;
};
