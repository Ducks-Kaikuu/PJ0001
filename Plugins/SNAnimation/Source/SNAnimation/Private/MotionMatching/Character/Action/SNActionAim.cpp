// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionAim.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

void USNActionAim::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		USNTraversalComponent* TraversalComponent(Player->GetTraversalComponent());

		if(TraversalComponent != nullptr)
		{
			TraversalComponent->SetWantsToAim(InputActionValue.Get<bool>());

			if(TraversalComponent->GetWantsToAim() == true)
			{
				TraversalComponent->SetWantsToStrafe(true);
			}
		}
	}
}
