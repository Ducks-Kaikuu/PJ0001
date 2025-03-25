// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJSpeedUpAction.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJ0000/Character/Components/SNComboComponent.h"
#include "System/PJGameInstance.h"
#include "System/PJTimerManager.h"
#include "Utility/SNUtility.h"

bool UPJSpeedUpAction::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	float InputValue = InputActionValue.Get<float>(), Value = 0.0f;
	
	Value = FMath::Clamp(1.0f + InputValue, 0.01f, UpperLimit);

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

	TimerManager->SetPlayerSpeedupRate(Value);

	TimerManager->UpdateTimer();

	return true;
}
