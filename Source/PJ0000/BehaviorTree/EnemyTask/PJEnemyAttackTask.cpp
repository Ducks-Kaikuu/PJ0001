// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyAttackTask.h"

#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "Character/Enemy/PJEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/SNUtility.h"

bool UPJEnemyAttackTask::ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool Result = Super::ExecAIAction(OwnerComp, NodeMemory);

	AAIController* AiController(Cast<AAIController>(OwnerComp.GetAIOwner()));

	if(AiController != nullptr)
	{
		APJEnemy* Character = Cast<APJEnemy>(AiController->GetPawn());

		if(Character != nullptr)
		{
			ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

			if (Player != nullptr)
			{
				float Distance = FVector::Distance(Character->GetActorLocation(), Player->GetActorLocation());

				Character->SetDistanceToPlayer(Distance);
			}
			
			if (GetActionTag().IsValid())
			{
				SetOwner(Character);

				Character->SetActinTag(GetActionTag());

				UPlayMontageCallbackProxy* MontagePlayProxy = Character->PlayAnimMontageByActionTag();

				if (MontagePlayProxy != nullptr)
				{
#if WITH_EDITORONLY_DATA
					if (bDebugDraw == true)
					{
						UKismetSystemLibrary::PrintString(GetWorld(), "Attacking");
					}
#endif
					if (MontagePlayProxy->OnCompleted.Contains(this, TEXT("OnEndplayMontage")) == false)
					{
						MontagePlayProxy->OnCompleted.AddDynamic(this, &UPJEnemyAttackTask::OnEndplayMontage);
					}

					if (MontagePlayProxy->OnInterrupted.Contains(this, TEXT("OnEndplayMontage")) == false)
					{
						MontagePlayProxy->OnInterrupted.AddDynamic(this, &UPJEnemyAttackTask::OnEndplayMontage);
					}
					
				} else
				{
					SNPLUGIN_LOG(TEXT("UPJEnemyAttackTask : Motion Can't Find."));

					Character->RemoveActionTag(GetActionTag());
					
					Result = false;
				}
			} else
			{
				Result = false;
			}
		}
	}
	
	return Result; 
}

void UPJEnemyAttackTask::OnEndplayMontage(FName NotifyName)
{
	ASNCharacterBase* Character = GetOwner<ASNCharacterBase>();

	if (Character != nullptr)
	{
		Character->RemoveActionTag(GetActionTag());
#if WITH_EDITORONLY_DATA
		if (bDebugDraw == true)
		{
			
			FString Name = Character->GetName();

			FString TagName = GetActionTag().ToString();

			FString ErrorStr = FString::Printf(TEXT("Remove Attack Tags.[%s] - [%s]"), *Name, *TagName);
			
			UKismetSystemLibrary::PrintString(GetWorld(), *ErrorStr);
		}
#endif
	}
}
