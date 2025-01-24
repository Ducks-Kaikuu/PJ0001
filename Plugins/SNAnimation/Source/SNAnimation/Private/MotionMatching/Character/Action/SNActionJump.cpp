// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionJump.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

bool USNActionJump::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		USNTraversalComponent* TraversalComponent(Player->GetTraversalComponent());

		if(TraversalComponent != nullptr)
		{
			if(TraversalComponent->IsDoingTraversalAction() != true)
			{
				UPawnMovementComponent* MovementComponent(Cast<UPawnMovementComponent>(Player->GetMovementComponent()));

				if(MovementComponent != nullptr)
				{
					if(MovementComponent->IsMovingOnGround())
					{
						float ForwardTraceDistance = TraversalComponent->GetTraversalForwardTraceDistance();

						bool bTraversalCheckFailed = false;
						bool bMontageSelectionFailed = false;
						
						TraversalComponent->ExecTraversalAction(ForwardTraceDistance, bTraversalCheckFailed, bMontageSelectionFailed);

						if(bTraversalCheckFailed == true)
						{
							Player->Jump();
						}
					}
				}
			}
		}
	} else
	{
		return false;
	}

	return true;
}
