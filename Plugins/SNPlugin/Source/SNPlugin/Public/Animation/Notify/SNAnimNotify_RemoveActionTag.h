// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SNAnimNotify_RemoveActionTag.generated.h"

//!@{@defgroup アニメーションノーティファイ
//!@{
/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNAnimNotify_RemoveActionTag : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Action|Tag")
	FGameplayTagContainer Tags;
};
//! @}
//! @}
