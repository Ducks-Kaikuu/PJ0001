// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/SNPlayerBase.h"
#include "SNMotionMatchingPlayerBase.generated.h"

class USNTraversalComponent;
class UMotionWarpingComponent;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SNANIMATION_API ASNMotionMatchingPlayerBase : public ASNPlayerBase
{
	GENERATED_BODY()

public:

	ASNMotionMatchingPlayerBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal")
	USNTraversalComponent* GetTraversalComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal")
	UMotionWarpingComponent* GetMotionWarpingComponent();
	
private:
	UPROPERTY(EditAnywhere, Category = "SNTraversal")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "SNTraversal")
	TObjectPtr<USNTraversalComponent> TraversalComponent = nullptr;
};

FORCEINLINE USNTraversalComponent* ASNMotionMatchingPlayerBase::GetTraversalComponent()
{
	return TraversalComponent;
}

FORCEINLINE UMotionWarpingComponent* ASNMotionMatchingPlayerBase::GetMotionWarpingComponent()
{
	return MotionWarpingComponent;
}