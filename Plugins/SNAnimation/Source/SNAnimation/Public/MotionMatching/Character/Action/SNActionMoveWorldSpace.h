// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "SNActionMoveWorldSpace.generated.h"

/**
 * 
 */
UCLASS()
class SNANIMATION_API USNActionMoveWorldSpace : public USNActionBase
{
	GENERATED_BODY()

public:
	virtual void ExecAction(const FInputActionValue& InputActionValue) override;
};
