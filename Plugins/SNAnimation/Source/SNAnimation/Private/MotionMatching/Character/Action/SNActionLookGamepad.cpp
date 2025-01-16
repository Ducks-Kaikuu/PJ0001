// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/Action/SNActionLookGamepad.h"

#include "Kismet/GameplayStatics.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"

void USNActionLookGamepad::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNMotionMatchingPlayerBase* Player(GetOwner<ASNMotionMatchingPlayerBase>());

	if(Player != nullptr)
	{
		FVector2D Vector(InputActionValue[0], InputActionValue[1]);
		
		Vector *= UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		Player->AddControllerYawInput(Vector.X);

		Player->AddControllerPitchInput(Vector.Y);
	}
}
