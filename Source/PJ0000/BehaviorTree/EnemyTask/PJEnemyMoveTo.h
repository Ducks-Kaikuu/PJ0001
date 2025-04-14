// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJEnemyMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJEnemyMoveTo : public USNActionBase
{
	GENERATED_BODY()

public:
	virtual bool ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void ExclusiveAction(const FGameplayTagContainer& Tag) override;

private:

	void SetOwnerSpeed(ACharacter* Character);

	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	float ChangeDistance = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Threshold =10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	float MinSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float MaxSpeed = 600.0f;
	

	UPROPERTY(EditAnywhere)
	FGameplayTag IdleTag;
	
	UPROPERTY(EditAnywhere)
	float ChaseDistance=100.0f;

	FVector TargetPosition = FVector::ZeroVector;
};


