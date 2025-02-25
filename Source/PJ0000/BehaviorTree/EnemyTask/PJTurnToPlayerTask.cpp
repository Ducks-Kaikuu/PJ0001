// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJTurnToPlayerTask.h"

#include "AIController.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Base/SNPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/SNUtility.h"

bool UPJTurnToPlayerTask::ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool Result = Super::ExecAIAction(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("UPJTurnToPlayerTask : AIController not found"));
		
		return false;
	}
	
	ASNCharacterBase* Character = Cast<ASNCharacterBase>(AIController->GetPawn());

	if (Character == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("UPJTurnToPlayerTask : Character not found"));
		
		return false;
	}

	if (Character->GetCharacterMovement()->IsMovingOnGround() == false)
	{
		return false;
	}

	ASNPlayerBase* Player = SNUtility::GetCurrentPlayer<ASNPlayerBase>();

	FRotator CurrentRotation(Character->GetActorRotation());
	
	FRotator TargetRotation =UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Player->GetActorLocation());

	if (FMath::Abs(TargetRotation.Yaw - CurrentRotation.Yaw) > LimitAngle)
	{
		FVector CurrentXVector(CurrentRotation.Vector());
		FVector TargetXVector (TargetRotation.Vector());
		
		FVector Axis(FVector::CrossProduct(CurrentXVector, TargetXVector));

		FVector Rotate = UKismetMathLibrary::RotateAngleAxis(CurrentXVector, LimitAngle, Axis);

		FRotator NewRotate = Rotate.Rotation();

		NewRotate.Pitch = 0.0f;
		
		Character->SetActorRotation(NewRotate);

		Result = false;
	} else
	{
		Result = true;
	}
	
	return Result;
}
