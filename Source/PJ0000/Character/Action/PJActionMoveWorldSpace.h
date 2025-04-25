// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionMatching/Character/Action/SNActionMoveWorldSpace.h"
#include "PJActionMoveWorldSpace.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJActionMoveWorldSpace : public USNActionMoveWorldSpace
{
	GENERATED_BODY()

public:

	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;
};
