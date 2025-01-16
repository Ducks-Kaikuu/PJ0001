// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionCrouch.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"

void USNActionCrouch::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		if(Player->bIsCrouched == true)
		{
			Player->UnCrouch();
		} else
		{
			Player->Crouch();
		}
	}
}
