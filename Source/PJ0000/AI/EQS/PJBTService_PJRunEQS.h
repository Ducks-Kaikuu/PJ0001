// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_RunEQS.h"
#include "PJBTService_PJRunEQS.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJBTService_PJRunEQS : public UBTService_RunEQS
{
	GENERATED_BODY()

public:
	
	UPJBTService_PJRunEQS(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void OnSNQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult);

protected:

	UPROPERTY(EditAnywhere, Category = "SN|Debug")
	bool bDebugDraw = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Score|Remap")
	float MinThreshold = 1.0f;

	UPROPERTY(EditAnywhere, Category = "SN|Score|Remap")
	float MaxThreshold = 2.0f;
};
