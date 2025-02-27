// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PJAnimNotifyState_GravityScale.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJAnimNotifyState_GravityScale : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Animation|Gravity")
	float GravityScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Animation|Gravity")
	float VelocityScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Animation|Gravity")
	bool bRestore = true;
	
	UPROPERTY()
	float OldGravityScale;
};
