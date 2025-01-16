// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionMatching/SNMotionMatchingDef.h"
#include "TraversalChooserParam.generated.h"

/** Chooserに設定する、評価に必要なパラメーター */
USTRUCT(BlueprintType)
struct FTraversalChooserParams
{
	GENERATED_BODY()

public:
	//!< トラバーサルアクションのタイプ
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TEnumAsByte<ETraversalActionType> ActionType = ETraversalActionType::None;

	//!< 移動状態
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EStride> Stride = EStride::Walk;

	//!< 移動速度
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed = 0.0f;

	//!< 障害物の高さ
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleHeight = 0.0f;

	//!< 奥にある障害物との距離
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ObstacleDepth = 0.0f;
	
};
