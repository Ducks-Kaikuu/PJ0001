// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PJAnimNotifyState_LoopCount.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJAnimNotifyState_LoopCount : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	int LoopNum = 5;
	
	UPROPERTY(EditAnywhere)
	FName NextSectionName = NAME_None;
};
