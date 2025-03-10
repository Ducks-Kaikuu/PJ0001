// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJSpeedUpAction.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJ0000/Character/Components/SNComboComponent.h"

bool UPJSpeedUpAction::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	float InputValue = InputActionValue.Get<float>();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f + InputValue);
	
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Speed up: %f"), InputValue), true, false);

	return true;
}
