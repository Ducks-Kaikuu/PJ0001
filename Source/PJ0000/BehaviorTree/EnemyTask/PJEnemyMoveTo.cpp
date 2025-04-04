// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyMoveTo.h"

#include "PJAIEnemy000.h"
#include "AI/EQS/PJEqsManager.h"
#include "AI/EQS/Utility/SNEqsLocationList.h"
#include "Character/Base/SNPlayerBase.h"
#include "Character/Enemy/PJEnemy.h"
#include "Character/Enemy/PJEnemyGroup.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "System/PJGameInstance.h"
#include "Utility/SNUtility.h"

bool UPJEnemyMoveTo::ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool Result = true; 

	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(OwnerComp.GetAIOwner());

	if (AIController != nullptr)
	{
#if 0
		APJEnemy* OwnerCharacter = Cast<APJEnemy>(AIController->GetPawn());

		ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

		if(Player != nullptr)
		{
			float DistanceToPlayer = FVector::Distance(OwnerCharacter->GetActorLocation(), Player->GetActorLocation());

			if (DistanceToPlayer < ChaseDistance)
			{
				MoveResult = EPathFollowingRequestResult::AlreadyAtGoal;
			}
		}
#endif

		UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

		USNEqsLocationList* EqsLocationList  = nullptr;
		
		if (GameInstance != nullptr)
		{
			UPJEqsManager* EqsManager = GameInstance->GetEqsManager();

			if (EqsManager != nullptr)
			{
				APJEnemy* OwnerCharacter = Cast<APJEnemy>(AIController->GetPawn());

				FGameplayTag EqsTag =UPJEnemyGroup::GetEqsName(OwnerCharacter->GetTeamID());

				EqsLocationList = EqsManager->GetEqsLocationList(EqsTag);
				
			}
		}
		
		if (EqsLocationList != nullptr)
		{
			TArray<FVector> EqsLocations(EqsLocationList->GetHighScoreLocations());

			if (EqsLocations.IsEmpty() == false)
			{
				int No = FMath::RandRange(0, EqsLocations.Num() - 1);

				TargetLocation = EqsLocations[No];

				APJEnemy* OwnerCharacter = Cast<APJEnemy>(AIController->GetPawn());

				float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), TargetLocation);
				
				float WalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
				
				if ((WalkSpeed == MinSpeed) && (Distance > ChangeDistance + Threshold))
				{
					OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
				} else
				if ((WalkSpeed == MaxSpeed) && (Distance < ChangeDistance - Threshold))
				{
					OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = MinSpeed;
				} else {
					if ((WalkSpeed != MaxSpeed) && (WalkSpeed != MaxSpeed))
					{
						OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = MinSpeed;
					}
				}

				EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation);

				ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

				if(Player != nullptr)
				{
					float DistanceToPlayer = FVector::Distance(OwnerCharacter->GetActorLocation(), Player->GetActorLocation());

					if (DistanceToPlayer < ChaseDistance)
					{
						MoveResult = EPathFollowingRequestResult::AlreadyAtGoal;
					}
				}

				if(MoveResult == EPathFollowingRequestResult::Type::Failed)
				{
					OwnerCharacter->RemoveActionTag(GetActionTag());
					OwnerCharacter->AddActionTag(IdleTag);
					
					Result = false;
				} else
				if(MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal){

					OwnerCharacter->RemoveActionTag(GetActionTag());
					OwnerCharacter->AddActionTag(IdleTag);
					
					Result = true;
				} else
				{
					OwnerCharacter->RemoveActionTag(IdleTag);
					OwnerCharacter->AddActionTag(GetActionTag());
					
					Result = false;
				}

				OwnerCharacter->PlayAnimMontageByActionTag();
				
				FLinearColor Color = FLinearColor::Yellow;

				
				
				UKismetSystemLibrary::DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 3, Color, OwnerCharacter->GetActorTickInterval(), 5.0f);
			}
		}
	}
	
	return Result;
}
