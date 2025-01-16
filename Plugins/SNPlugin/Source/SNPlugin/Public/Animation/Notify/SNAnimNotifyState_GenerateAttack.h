// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SNAnimNotifyState_GenerateAttack.generated.h"

/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNAnimNotifyState_GenerateAttack : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	FName ParentSocket = NAME_None;

	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bIgnoreRotation = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bSweep = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bPenetrate = false;

	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bBomb = false;

	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	float Time = 0.0f;

	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	float Radius = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "!bSweep"))
	FVector Position;
	
	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "bSweep"))
	FVector Start;

	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "bSweep"))
	FVector End;

	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	FGameplayTagContainer DamageAttribetes;
};
