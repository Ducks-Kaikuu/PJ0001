// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PJAnimNotify_PauseAndJumpSection.generated.h"

class ASNCharacterBase;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJAnimNotify_PauseAndJumpSection : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	UPROPERTY(EditAnywhere)
	FName NextSectionName = NAME_None;

	UPROPERTY(EditAnywhere)
	float LoopTime = 0.0f;
};
