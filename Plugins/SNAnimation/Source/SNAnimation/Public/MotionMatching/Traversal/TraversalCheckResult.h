// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionMatching/SNMotionMatchingDef.h"
#include "TraversalCheckResult.generated.h"

/** トラバーサルアクション判定の結果 */
USTRUCT(BlueprintType)
struct FTraversalCheckResult
{
	GENERATED_BODY()

public:
	//!< トラバーサルアクションのタイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETraversalActionType> ActionType = ETraversalActionType::None;

	//!< 障害物にフロントレッジがあるか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFrontLedge = false;

	//!< フロントレッジの位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FrontLedgeLocation = FVector::ZeroVector;

	//!< フロントレッジの法線
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FrontLedgeNormal = FVector::ZeroVector;

	//!< 障害物にバックレッジがあるか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackLedge = false;

	//!< バックレッジの位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackLedgeLocation = FVector::ZeroVector;

	//!< バックレッジの法線
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackLedgeNormal = FVector::ZeroVector;

	//!< 障害物にバックフロアがあるか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackFloor = false;

	//!< バックフロアの位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackFloorLocation = FVector::ZeroVector;

	//!< 障害物の高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleHeight = 0.0f;

	//!< 奥にある障害物との距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleDepth = 0.0f;

	//!< バックレッジの高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackLedgeHeight = 0.0f;

	//!< 衝突したコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPrimitiveComponent> HitComponent;

	//!< 選択されたモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ChosenMontage;

	//!< 再生開始時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime = 0.0f;

	//!< 再生速度倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate = 1.0f;
};
