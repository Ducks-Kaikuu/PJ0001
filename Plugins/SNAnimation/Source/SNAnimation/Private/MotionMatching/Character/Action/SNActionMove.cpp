// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionMove.h"

#include "Kismet/KismetMathLibrary.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

bool USNActionMove::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		FRotator Rotator(Player->GetControlRotation());

		FVector RightVector(UKismetMathLibrary::GetRightVector(FRotator(0.0f, Rotator.Yaw, 0.0f)));

		Player->AddMovementInput(RightVector, InputActionValue[0]);

		FVector ForwardVector(UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Rotator.Yaw, 0.0f)));

		Player->AddMovementInput(ForwardVector, InputActionValue[1]);

		if(USNTraversalComponent* TraversalComponent= Player->GetTraversalComponent())
		{
			TraversalComponent->SetStride(InputActionValue.Get<FVector2D>());

			return true;
		}
	}

	return false;
}
