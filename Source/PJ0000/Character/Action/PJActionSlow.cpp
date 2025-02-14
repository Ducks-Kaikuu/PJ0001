// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJActionSlow.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

bool UPJActionSlow::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	float InputValue = InputActionValue.Get<float>(), Value = 0.0f;

	Value = FMath::Clamp(1.0f - InputValue, SlowSpeed, 1.0f);
	
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Value);

	ACharacter* Character = GetOwner<ACharacter>();

	Character->CustomTimeDilation = Value;

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Speed Down: %f"), InputValue), true, false);

	return true;
}
