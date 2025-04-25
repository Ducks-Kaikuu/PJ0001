// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Action/PJActionMoveWorldSpace.h"

#include "Character/PJCharacter.h"

bool UPJActionMoveWorldSpace::ExecAction(const FInputActionValue& InputActionValue)
{
	bool Result =Super::ExecAction(InputActionValue);

	APJCharacter* Player = GetOwner<APJCharacter>();

	if (Player != nullptr)
	{
		FVector2D MoveInput = InputActionValue.Get<FVector2D>();
		
		Player->SetMoveInput(MoveInput);
	}
	
	return Result;
}
