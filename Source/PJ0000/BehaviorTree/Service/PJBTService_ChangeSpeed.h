// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "PJBTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJBTService_ChangeSpeed : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	UPJBTService_ChangeSpeed(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float ChangeDistance = 500.0f;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	float MinSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float MaxSpeed = 600.0f;
};
