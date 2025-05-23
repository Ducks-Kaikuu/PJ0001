// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJCameraAction.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJCameraAction : public USNActionBase
{
	GENERATED_BODY()

public:
	
	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;
};
