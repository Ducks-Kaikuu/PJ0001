// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Scene/StateTree/PJBattleSceneTask.h"

#include "PJ0000/UI/Widget/PJBattleUI.h"

EStateTreeRunStatus UPJBattleSceneTask::EnterState(FStateTreeExecutionContext& Context,
                                                   const FStateTreeTransitionResult& Transition)
{
	return Super::EnterState(Context, Transition);
}

void UPJBattleSceneTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}

void UPJBattleSceneTask::HudPostLoad()
{
	Super::HudPostLoad();

	UPJBattleUI* Menu = GetHudWidget<UPJBattleUI>();

	if (Menu != nullptr)
	{
		SetVisibleWidget(EWidgetLayer::Layer3, Menu);
	}
}
