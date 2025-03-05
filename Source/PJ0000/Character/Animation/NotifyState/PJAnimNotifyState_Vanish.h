// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PJAnimNotifyState_Vanish.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJAnimNotifyState_Vanish : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;
};
