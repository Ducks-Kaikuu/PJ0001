// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionMatching/Character/Action/SNActionMove.h"
#include "PJActionMove.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJActionMove : public USNActionMove
{
	GENERATED_BODY()

public:

	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;
};
