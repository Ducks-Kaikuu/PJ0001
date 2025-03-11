// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PJCameraActorBase.h"
#include "PJBattleCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class PJ0000_API APJBattleCameraActor : public APJCameraActorBase
{
	GENERATED_BODY()

public:

	FRotator GetCameraRotator() const ;

	void SetCameraRotator(const FRotator& Rotator);

	float GetXAxisSpeed() const ;

	float GetYAxisSpeed() const ;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:

	//!< アタッチするボーン名
	UPROPERTY(EditAnyWhere, Category="PJ|Camera|Parameter")
	FName BoneName=NAME_None;
	
	//!< ターゲットからのオフセット情報
	UPROPERTY(EditAnyWhere, Category="PJ|Camera|Parameter")
	FVector TargetOffset=FVector(845.0f, 0.0f, 300.0f);
	
	//!< 車をフォーカスしたカメラ
	UPROPERTY(EditAnywhere, Category="PJ|Camera|Parameter")
	bool bCarFocusedCamera = false;
	
	//!< 操作時のスピード
	UPROPERTY(EditAnywhere, Category="PJ|Camera|Parameter")
	float XAxisSpeed = 1.0f;
	
	//!< 操作時のスピード
	UPROPERTY(EditAnywhere, Category="PJ|Camera|Parameter")
	float YAxisSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="PJ|Camera|Parameter")
	float FocuedCameraHeight = 200.0f;

	UPROPERTY(EditAnywhere, Category="PJ|Camera|Parameter")
	bool bCameraTick = true;
	
	//!< カメラの回転情報
	UPROPERTY()
	FRotator CameraRotate = FRotator::ZeroRotator;

	UPROPERTY()
	FTransform OldPlayerTransform;
	
	UPROPERTY()
	FTransform CurrentPlayerTransform;
};

//----------------------------------------------------------------------//
//
//! @brief カメラのローテーション情報を取得
//
//! @retval カメラのローテーション情報
//
//----------------------------------------------------------------------//
FORCEINLINE FRotator APJBattleCameraActor::GetCameraRotator() const {
	return CameraRotate;
}

//----------------------------------------------------------------------//
//
//! @brief カメラのローテーション情報を設定
//
//! @param Rotator カメラのローテーション情報
//
//----------------------------------------------------------------------//
FORCEINLINE void APJBattleCameraActor::SetCameraRotator(const FRotator& Rotator){
	CameraRotate = Rotator;
}

//----------------------------------------------------------------------//
//
//! @brief 入力時のX方向の移動スピードを取得
//
//! @retval 入力時のX方向の移動スピード
//
//----------------------------------------------------------------------//
FORCEINLINE float APJBattleCameraActor::GetXAxisSpeed() const {
	return XAxisSpeed;
}

//----------------------------------------------------------------------//
//
//! @brief 入力時のY方向の移動スピードを取得
//
//! @retval 入力時のY方向の移動スピード
//
//----------------------------------------------------------------------//
FORCEINLINE float APJBattleCameraActor::GetYAxisSpeed() const {
	return YAxisSpeed;
}

