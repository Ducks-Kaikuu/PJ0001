// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJEnemyAttackTask.h"

#include "AIController.h"
#include "PlayMontageCallbackProxy.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/NPC/PJEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

bool UPJEnemyAttackTask::ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool Result = Super::ExecAIAction(OwnerComp, NodeMemory);

	AAIController* AiController(Cast<AAIController>(OwnerComp.GetAIOwner()));

	if(AiController != nullptr)
	{
		APJEnemy* Character = Cast<APJEnemy>(AiController->GetPawn());

		if(Character != nullptr)
		{
			if (GetActionTag().IsValid() && Character->HasActionTag(GetActionTag()) == false)
			{
				SetOwner(Character);

				Character->SetActinTag(GetActionTag());

				UPlayMontageCallbackProxy* MontagePlayProxy = Character->PlayAnimMontageByActionTag();

				if (MontagePlayProxy != nullptr)
				{
					MontagePlayProxy->OnCompleted.AddDynamic(this, &UPJEnemyAttackTask::OnEndplayMontage);
					MontagePlayProxy->OnInterrupted.AddDynamic(this, &UPJEnemyAttackTask::OnEndplayMontage);
				} else
				{
					Result = false;
				}
			} else
			{
				Result = false;
			}
		}
	}
	
#if WITH_EDITORONLY_DATA
	if (bDebugDraw == true)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Attacking");
	}
#endif
	
	return Result; 
}

void UPJEnemyAttackTask::OnEndplayMontage(FName NotifyName)
{
	ASNCharacterBase* Character = GetOwner<ASNCharacterBase>();

	if (Character != nullptr)
	{
		Character->RemoveActionTag(GetActionTag());
	}
}
