// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyMoveTo.h"

#include "PJAIEnemy000.h"
#include "SNDef.h"
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
	// AIControllerを取得
	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(OwnerComp.GetAIOwner());
	
	if(AIController != nullptr){
		
		ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());
		
		if(Player != nullptr){
			// プレイヤーと目的地の距離を算出
			float DistanceToPlayer = FVector::Distance(Player->GetActorLocation(), TargetPosition);
			// 目的地が原点(初期値)ではなく、かつ、プレイヤーに近い位置かチェック
			if((TargetPosition == FVector::ZeroVector) || (DistanceToPlayer >= ChaseDistance)){
				
				USNEqsLocationList* EqsLocationList(nullptr);
				// GameInstanceを取得
				if(UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>()){
					// EQSManagerを取得
					if(UPJEqsManager* EqsManager = GameInstance->GetEqsManager()){
						// オーナーキャラクターを取得
						if(APJEnemyBase* OwnerCharacter = Cast<APJEnemyBase>(AIController->GetPawn())){
							// TeamIDからタグを取得(TeamIDと紐づけて大丈夫なのか…。要検討。)
							FGameplayTag EqsTag =UPJEnemyGroup::GetEqsName(OwnerCharacter->GetTeamID());
							// EQSの結果を取得
							EqsLocationList = EqsManager->GetEqsLocationList(EqsTag);
						}
					}
				}
				// EQSの結果がない場合はプレイヤーの位置を目的地とする
				if(EqsLocationList == nullptr){
					TargetPosition = Player->GetActorLocation();
				} else {
					// スコアが高い位置情報を取得
					TArray<FVector> EqsLocations(EqsLocationList->GetHighScoreLocations());
					
					if(EqsLocations.Num() > 0){
						
						int Index = FMath::RandRange(0, EqsLocations.Num() - 1);
						// 目的地を設定
						TargetPosition = EqsLocations[Index];

						EqsLocationList->RemoveAt(Index);
					} else {
						TargetPosition = Player->GetActorLocation();
					}
				}
			}
			// オーナーキャラクターを取得
			if(APJEnemyBase* OwnerCharacter = Cast<APJEnemyBase>(AIController->GetPawn())){
				// 移動スピードを設定
				SetOwnerSpeed(OwnerCharacter);
				// 目的地へ移動
				EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetPosition);
				
				if(MoveResult == EPathFollowingRequestResult::Type::Failed){
					
					OwnerCharacter->RemoveActionTag(GetActionTag());
					OwnerCharacter->AddActionTag(IdleTag);
					
					TargetPosition = FVector::ZeroVector;
					
					Result = true;
				} else
				if(MoveResult == EPathFollowingRequestResult::Type::AlreadyAtGoal){
					
					OwnerCharacter->RemoveActionTag(GetActionTag());
					OwnerCharacter->AddActionTag(IdleTag);
					
					Result = true;
				} else {
					OwnerCharacter->RemoveActionTag(IdleTag);
					OwnerCharacter->AddActionTag(GetActionTag());
					
					Result = false;
				}
				
				OwnerCharacter->PlayAnimMontageByActionTag();
				
				FLinearColor Color = FLinearColor::Yellow;
				
				UKismetSystemLibrary::DrawDebugSphere(GetWorld(), TargetPosition, 50.0f, 3, Color, OwnerCharacter->GetActorTickInterval(), 5.0f);
			}
		}
	}
	
	return Result;
}

void UPJEnemyMoveTo::SetOwnerSpeed(ACharacter* Character){
	
	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement()){
		// 目的地までの距離を算出
		float Distance = FVector::Distance(Character->GetActorLocation(), TargetPosition);
		// 現在の歩行スピードを取得
		float WalkSpeed = MovementComponent->MaxWalkSpeed;
		// レンジチェック
		if((WalkSpeed == MinSpeed) && (Distance > ChangeDistance + Threshold)){
			MovementComponent->MaxWalkSpeed = MaxSpeed;
		} else
		if((WalkSpeed == MaxSpeed) && (Distance < ChangeDistance - Threshold)){
			MovementComponent->MaxWalkSpeed = MinSpeed;
		} else {
			if((WalkSpeed != MaxSpeed) && (WalkSpeed != MinSpeed)){
				MovementComponent->MaxWalkSpeed = MinSpeed;
			}
		}
	}
}

void UPJEnemyMoveTo::ExclusiveAction(const FGameplayTagContainer& Tag)
{
	Super::ExclusiveAction(Tag);

	ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());
		
	if(Player != nullptr)
	{
		// プレイヤーと目的地の距離を算出
		float DistanceToPlayer = FVector::Distance(Player->GetActorLocation(), TargetPosition);

		if(DistanceToPlayer >= ChaseDistance)
		{
			TargetPosition = FVector::ZeroVector;
		}
	}
}
