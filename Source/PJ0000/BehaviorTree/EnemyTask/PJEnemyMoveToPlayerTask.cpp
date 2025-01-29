// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyMoveToPlayerTask.h"

#include "AIController.h"
#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utility/SNUtility.h"

EBTNodeResult::Type UPJEnemyMoveToPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =Super::ExecuteTask(OwnerComp, NodeMemory); 

	AAIController* AiController(Cast<AAIController>(OwnerComp.GetAIOwner()));

	if(AiController != nullptr)
	{
			ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

			if(Player != nullptr)
			{
				EPathFollowingRequestResult::Type MoveResult = AiController->MoveToActor(Player, ChaseDistance);

				if(MoveResult == EPathFollowingRequestResult::Type::Failed)
				{
					Result = EBTNodeResult::Type::Failed;
				} else
					if(MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal){
						Result = EBTNodeResult::Type::Succeeded;

						SNPLUGIN_LOG(TEXT("Move Task is Finished."));
					} else
					{
						Result = EBTNodeResult::Type::InProgress;
					}
			}
	}

	return Result;
}
