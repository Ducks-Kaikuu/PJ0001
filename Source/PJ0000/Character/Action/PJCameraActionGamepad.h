// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJCameraActionGamepad.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJCameraActionGamepad : public USNActionBase
{
	GENERATED_BODY()

	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;
};
