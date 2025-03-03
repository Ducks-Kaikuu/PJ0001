// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Service/PJBTService_ChangeSpeed.h"

#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Character/Base/SNPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UPJBTService_ChangeSpeed::UPJBTService_ChangeSpeed(const FObjectInitializer& ObjectInitializer)
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UPJBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UPJBTService_ChangeSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
