// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SNAnimNotifyState_MovePosition.generated.h"

class UCurveVector;

//!@{@defgroup アニメーションノーティファイステート
//!@{
//----------------------------------------------------------------------//
//
//! @brief アニメーション中に移動させるための処理
//
//!USNMovePositionComponentとセットで使用してください。
//!(キャラクターにUSNMovePositionComponentを持たせてください。)
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNAnimNotifyState_MovePosition : public UAnimNotifyState
{
	GENERATED_BODY()
	
	//! @{@name ノーティファイの開始
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//! @}
	
	//! @{@name ノーティファイ中に呼ばれるティック処理
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	//! @}
	
	//! @{@name ノーティファイの終了
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//! @}
	
private:

	float CalculateNotifyStateProgressPercentage(float StartTime, float EndTime, float CurrentTime);
	
	//!< トランスフォーム名
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	FName TransformName;
	
	//!< 現在のトランスフォームに加算するかどうかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	bool bAdditive = false;
	
	//!< ポジション情報を移動するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	bool bMoveLocation = true;
	
	//!< カーブ情報を絶対値として使用するか絶対値として使用するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveLocation"))
	bool bUsePositionCurveAsAbsoluteValue = true;
	
	//!< カーブ情報を相対値として使用する場合の最終目的地
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "!bUsePositionCurveAsAbsoluteValue"))
	FVector TargetPosition;
	
	//!< ポジションのカーブ情報
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveLocation"))
	TObjectPtr<UCurveVector> TranslateInterpolate = nullptr;
	
	//!< ローテーション情報を移動するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState")
	bool bMoveRotation = true;
	
	//!< カーブ情報を絶対値として使用するか絶対値として使用するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveRotation"))
	bool bUseRotationCurveAsAbsoluteValue = true;
	
	//!< カーブ情報を相対値として使用する場合の最終目的地
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "!bUseRotationCurveAsAbsoluteValue"))
	FRotator TargetRotation;
	
	//!< ローテーションのカーブ情報
	UPROPERTY(EditAnywhere, Category = "SN|Animation|NotifyState", meta = (EditCondition = "bMoveRotation"))
	TObjectPtr<UCurveVector> RotateInterpolate = nullptr;
};
//! @}
//! @}
