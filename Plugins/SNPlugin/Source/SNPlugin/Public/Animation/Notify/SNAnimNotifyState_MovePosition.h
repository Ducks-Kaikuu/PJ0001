// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SNAnimNotifyState_MovePosition.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNAnimNotifyState_MovePosition : public UAnimNotifyState
{
	GENERATED_BODY()

	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:

	float CalculateNotifyStateProgressPercentage(float StartTime, float EndTime, float CurrentTime);
	
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	FName TransformName;

	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	bool bAdditive = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	bool bMoveLocation = true;

	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveLocation"))
	bool bUseCurveAsAbsoluteValue = true;
	
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "!bUseCurveAsAbsoluteValue"))
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveLocation"))
	TObjectPtr<UCurveVector> TranslateInterpolate = nullptr;

	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	TObjectPtr<UCurveVector> RotateInterpolate = nullptr;
};
