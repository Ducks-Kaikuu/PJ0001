// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJActionSlow.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/PJGameInstance.h"
#include "System/PJTimerManager.h"
#include "Utility/SNUtility.h"

bool UPJActionSlow::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance == nullptr)
	{
		return false;
	}

	UPJTimerManager* TimerManager = GameInstance->GetTimerManager();

	if (TimerManager == nullptr)
	{
		return false;
	}

	float InputValue = InputActionValue.Get<float>(), Value = 0.0f;
	
	Value = FMath::Clamp(1.0f - InputValue, SlowSpeed, 1.0f);

	TimerManager->SetWorldTimerRate(Value);

	TimerManager->UpdateTimer();
	
	return true;
}
