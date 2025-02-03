// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyMoveToPlayerTask.h"

#include "AIController.h"
#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "Character/NPC/PJEnemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utility/SNUtility.h"

EBTNodeResult::Type UPJEnemyMoveToPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result =Super::ExecuteTask(OwnerComp, NodeMemory); 

	AAIController* AiController(Cast<AAIController>(OwnerComp.GetAIOwner()));
	
	if(AiController != nullptr){
		
		APJEnemy* Character = Cast<APJEnemy>(AiController->GetPawn());
		
		if(Character != nullptr)
		{
			ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

			if(Player != nullptr)
			{
				EPathFollowingRequestResult::Type MoveResult = AiController->MoveToActor(Player, ChaseDistance, false, false);

				if(MoveResult == EPathFollowingRequestResult::Type::Failed)
				{
					Character->RemoveActionTag(WalkTag);
					Character->AddActionTag(IdleTag);
					
					Result = EBTNodeResult::Type::Failed;
#if WITH_EDITORONLY_DATA
					if (bDebugDraw == true)
					{
						SNPLUGIN_LOG(TEXT("Move Task is Failed."));
					}
#endif
				} else
				if(MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal){

					Character->RemoveActionTag(WalkTag);
					Character->AddActionTag(IdleTag);
					
					Result = EBTNodeResult::Type::Succeeded;
#if WITH_EDITORONLY_DATA
					if (bDebugDraw == true)
					{
						SNPLUGIN_LOG(TEXT("Move Task is Finished."));
					}
#endif
				} else
				{
					Character->RemoveActionTag(IdleTag);
					Character->AddActionTag(WalkTag);
					
					Result = EBTNodeResult::Type::InProgress;
				}
				
				Character->PlayAnimMontageByActionTag();
			}
		}
	}

	return Result;
}
