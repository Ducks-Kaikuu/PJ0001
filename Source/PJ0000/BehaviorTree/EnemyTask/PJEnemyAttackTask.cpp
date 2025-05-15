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
		APJEnemyBase* Character = Cast<APJEnemyBase>(AiController->GetPawn());

		if(Character != nullptr)
		{
			if (GetActionTag().IsValid())
			{
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
					Character->SetAttackMotionDelegate(MontagePlayProxy, GetActionTag());
#if WITH_EDITORONLY_DATA
					if (bDebugDraw == true)
					{
			
						FString Name = Character->GetActorLabel();

						FString TagName = GetActionTag().ToString();

						FString ErrorStr = FString::Printf(TEXT("Add Attack Tags.[%s] - [%s]"), *Name, *TagName);
			
						UKismetSystemLibrary::PrintString(GetWorld(), *ErrorStr);
					}
#endif
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
	APJEnemyBase* Character = GetOwner<APJEnemyBase>();

	if (Character != nullptr)
	{
		Character->RemoveActionTag(GetActionTag());

		Character->ResetMontagePlayProxy();
#if WITH_EDITORONLY_DATA
		if (bDebugDraw == true)
		{
			
			FString Name = Character->GetActorLabel();

			FString TagName = GetActionTag().ToString();

			FString ErrorStr = FString::Printf(TEXT("Remove Attack Tags.[%s] - [%s]"), *Name, *TagName);
			
			UKismetSystemLibrary::PrintString(GetWorld(), *ErrorStr);
		}
#endif
	} else
	{
		SNPLUGIN_ERROR(TEXT("[UPJEnemyAttackTask::OnEndplayMontage] - Character is Null."));
	}
}
