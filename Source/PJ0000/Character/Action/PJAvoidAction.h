// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/SNActionBase.h"
#include "PJAvoidAction.generated.h"

class UChooserTable;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJAvoidAction : public USNActionBase
{
	GENERATED_BODY()

public:

	virtual bool ExecAction(const FInputActionValue& InputActionValue) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Action|Dodge")
	float AxisX;

private:
	UPROPERTY(EditAnywhere, Category = "Action|Action|Avoid|Animation")
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, Category = "Action|Action|Dodge|Animation")
	TObjectPtr<UChooserTable> DodgeChooserTable = nullptr;
};
