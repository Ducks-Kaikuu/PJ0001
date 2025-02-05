// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionLook.h"

#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"

void USNActionLook::Initialize(UEnhancedInputComponent* InputComponent, const UInputAction* InputAction, UObject* Object)
{
	Super::Initialize(InputComponent, InputAction, Object);

	ActionTag = FGameplayTag::RequestGameplayTag(TEXT("Input.Normal.SNLook"));
}

bool USNActionLook::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		Player->AddControllerYawInput(InputActionValue[0]);

		Player->AddControllerPitchInput(InputActionValue[1]);
	} else
	{
		return false;
	}

	return true;
}
