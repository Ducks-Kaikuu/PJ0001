// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionMoveWorldSpace.h"

#include "Kismet/KismetMathLibrary.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

void USNActionMoveWorldSpace::Initialize(UEnhancedInputComponent* InputComponent, const UInputAction* InputAction, UObject* Object)
{
	Super::Initialize(InputComponent, InputAction, Object);

	ActionTag = FGameplayTag::RequestGameplayTag(TEXT("Input.Normal.SNMove_WorldSpace"));
}

bool USNActionMoveWorldSpace::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr){

		FVector2D NormalizedVector(UKismetMathLibrary::Normal2D(FVector2D(InputActionValue[0], InputActionValue[1])));

		Player->AddMovementInput(FVector(0.0f, 1.0f, 0.0f), NormalizedVector.X);

		Player->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), NormalizedVector.Y);

		if(USNTraversalComponent* TraversalComponent = Player->GetTraversalComponent())
		{
			TraversalComponent->SetStride(InputActionValue.Get<FVector2D>());

			return true;
		}
	}

	return false;
}
