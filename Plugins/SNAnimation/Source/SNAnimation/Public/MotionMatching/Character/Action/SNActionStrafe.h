﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "SNActionStrafe.generated.h"

/**
 * 
 */
UCLASS()
class SNANIMATION_API USNActionStrafe : public USNActionBase
{
	GENERATED_BODY()

public:
	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;
};
