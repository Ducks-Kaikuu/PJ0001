// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionSprint.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

bool USNActionSprint::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		USNTraversalComponent* TraversalComponent(Player->GetTraversalComponent());

		if(TraversalComponent != nullptr)
		{
			TraversalComponent->SetWantsToSprint(InputActionValue.Get<bool>());

			TraversalComponent->SetWantsToWalk(false);

			return true;
		}
	}

	return false;
}
