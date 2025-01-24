// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJUpperAttackAction.generated.h"

class UPlayMontageCallbackProxy;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJUpperAttackAction : public USNActionBase
{
	GENERATED_BODY()

public:
	
	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;

	UFUNCTION()
	void OnMontagePlayEnd(FName NotifyName);
	
private:
	UPROPERTY()
	UPlayMontageCallbackProxy* PlayMontage = nullptr;
};
