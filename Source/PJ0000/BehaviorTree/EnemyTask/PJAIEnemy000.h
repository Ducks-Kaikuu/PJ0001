// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PJAIEnemy000.generated.h"

class UBehaviorTreeComponent;
class ASNPlayerBase;
/**
 * 
 */
UCLASS()
class PJ0000_API APJAIEnemy000 : public AAIController
{
	GENERATED_BODY()

public:
	APJAIEnemy000(const FObjectInitializer& ObjectInitializer);

	void SetPlayerKey(APawn* PlayerPawn);

	UFUNCTION(BlueprintCallable)
	ASNPlayerBase* GetPlayerKey();

	UBehaviorTreeComponent* GetBehaviorTreeComponent();

	virtual UBlackboardComponent* GetBlackboardComponent();

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsBehaviorTreeRunning() const ;
	
	void Restart();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	FName PlayerKeyName = FName(TEXT("Player"));
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent = nullptr;
};

FORCEINLINE UBehaviorTreeComponent* APJAIEnemy000::GetBehaviorTreeComponent()
{
	return BehaviorComponent;
}

FORCEINLINE UBlackboardComponent* APJAIEnemy000::GetBlackboardComponent()
{
	return BlackboardComponent;
}

