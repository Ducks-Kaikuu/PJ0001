// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/StateTree/SNSceneStateTreeTaskBase.h"
#include "PJBattleSceneTask.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API UPJBattleSceneTask : public USNSceneStateTreeTaskBase
{
	GENERATED_BODY()
	
protected:
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	virtual void HudPostLoad() override;
};
