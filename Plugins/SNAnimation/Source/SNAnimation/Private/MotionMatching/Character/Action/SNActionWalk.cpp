// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionWalk.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

void USNActionWalk::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		USNTraversalComponent* TraversalComponent(Player->GetTraversalComponent());

		if(TraversalComponent != nullptr)
		{
			bool bWantsToSprint = TraversalComponent->GetWantsToSprint();

			if(bWantsToSprint != true)
			{
				bool bWantsToWalk = TraversalComponent->GetWantsToWalk() != true;

				TraversalComponent->SetWantsToWalk(bWantsToWalk);
			}
		}
	}
}
