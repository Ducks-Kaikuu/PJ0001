// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJRunEQSTask.h"

void UPJRunEQSTask::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UPJRunEQSTask::OnPJQueryFinished);
}

EBTNodeResult::Type UPJRunEQSTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerBTComp = OwnerComp;
	
	return Result;
}

void UPJRunEQSTask::OnPJQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	
}
