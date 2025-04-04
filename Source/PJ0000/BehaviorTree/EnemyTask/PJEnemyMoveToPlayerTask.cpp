// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyMoveToPlayerTask.h"

#include "AIController.h"
#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "Character/Enemy/PJEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utility/SNUtility.h"

bool UPJEnemyMoveToPlayerTask::ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool Result = true;
	
	AAIController* AiController(Cast<AAIController>(OwnerComp.GetAIOwner()));
	
	if(AiController != nullptr){
		
		APJEnemy* Character = Cast<APJEnemy>(AiController->GetPawn());
		
		if(Character != nullptr)
		{
			ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

			if(Player != nullptr)
			{
				float Distance = FVector::Distance(Character->GetActorLocation(), Player->GetActorLocation());

				float WalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
				
				if ((WalkSpeed == MinSpeed) && (Distance > ChangeDistance + Threshold))
				{
					Character->GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
				} else
				if ((WalkSpeed == MaxSpeed) && (Distance < ChangeDistance - Threshold))
				{
					Character->GetCharacterMovement()->MaxWalkSpeed = MinSpeed;
				} else
				{
					if ((WalkSpeed != MaxSpeed) && (WalkSpeed != MaxSpeed))
					{
						Character->GetCharacterMovement()->MaxWalkSpeed = MinSpeed;
					}
				}
				
				EPathFollowingRequestResult::Type MoveResult = AiController->MoveToActor(Player, ChaseDistance, false, false);

#if WITH_EDITORONLY_DATA
				if (bDebugDraw == true)
				{
					FString TmpString(TEXT(""));
					
					switch (MoveResult)
					{
						case EPathFollowingRequestResult::Failed: TmpString = TEXT("EPathFollowingRequestResult::Failed"); break;
						case EPathFollowingRequestResult::RequestSuccessful: TmpString = TEXT("EPathFollowingRequestResult::RequestSuccessful"); break;
						case EPathFollowingRequestResult::AlreadyAtGoal: TmpString = TEXT("EPathFollowingRequestResult::AlreadyAtGoal"); break;
						default: TmpString = TEXT("Unknown PathFollowingRequestResult"); break;
					}
					
					SNPLUGIN_LOG(TEXT("MoveAI : %s"), *TmpString);	
				}
#endif
				
				
				if(MoveResult == EPathFollowingRequestResult::Type::Failed)
				{
					Character->RemoveActionTag(GetActionTag());
					Character->AddActionTag(IdleTag);
					
					Result = false;
#if WITH_EDITORONLY_DATA
					if (bDebugDraw == true)
					{
						SNPLUGIN_LOG(TEXT("Move Task is Failed."));
					}
#endif
				} else
					if(MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal){

						Character->RemoveActionTag(GetActionTag());
						Character->AddActionTag(IdleTag);
					
						Result = true;
#if WITH_EDITORONLY_DATA
						if (bDebugDraw == true)
						{
							SNPLUGIN_LOG(TEXT("Move Task is Finished."));
						}
#endif
					} else
					{
						Character->RemoveActionTag(IdleTag);
						Character->AddActionTag(GetActionTag());
					
						Result = false;
					}
				
				Character->PlayAnimMontageByActionTag();
			}
		}
	}
	
	return Result;
}
