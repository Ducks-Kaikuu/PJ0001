// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PJAnimNotifyState_PlayRate.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJAnimNotifyState_PlayRate : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<UCurveFloat> PlayRateCurve =nullptr;

	UPROPERTY(EditAnywhere, Category = Animation, Meta = (UIMin=0.1f, UIMax=5.f))
	float PlayRate = 1.0f;
};
