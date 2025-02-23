﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionStrafe.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

bool USNActionStrafe::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		USNTraversalComponent* TraversalComponent(Player->GetTraversalComponent());

		if(TraversalComponent != nullptr)
		{
			bool bWantsToStrafe = TraversalComponent->GetWantsToStrafe();

			TraversalComponent->SetWantsToStrafe(bWantsToStrafe != true);

			return true;
		}
	}

	return false;
}
