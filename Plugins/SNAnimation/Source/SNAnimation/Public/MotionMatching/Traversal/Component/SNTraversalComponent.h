// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "MotionMatching/SNMotionMatchingDef.h"
#include "MotionMatching/Traversal/TraversalCheckResult.h"
#include "SNTraversalComponent.generated.h"


class UChooserTable;
class UPlayMontageCallbackProxy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNANIMATION_API USNTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNTraversalComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	float GetTraversalForwardTraceDistance() const;
	
	void ExecTraversalAction(float TraceForwardDistance, bool& TraversalCheckFailed, bool& MontageSelectionFailed);
	
	//! @{@name 歩幅状態を設定する
	void SetStride(const FVector2D& Input);
	//! @}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	bool GetWantsToSprint() const ;
	
	void SetWantsToSprint(bool Flag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	bool GetWantsToWalk() const ;
	
	void SetWantsToWalk(bool Flag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	bool GetWantsToStrafe() const ;
	
	void SetWantsToStrafe(bool Flag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	bool GetWantsToAim() const ;
	
	void SetWantsToAim(bool Flag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	bool IsJustLanded() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SN|MotionMatching")
	const FVector& GetLandSpeed() const ;
	
	UFUNCTION(BlueprintCallable, Category = "SN|Traversal")
	bool IsDoingTraversalAction() const ;
	
	EStride GetStride() const ;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	
	//! @{@name 着地時の処理
	UFUNCTION()
	void OnLanded(const FHitResult& Hit);
	//! @}
	
	//! @{@name 着地フラグを下す
	UFUNCTION()
	void TurnOffJustLandded();
	//! @}
	
	//! @{@name モンタージュの再生が終了した際のデリゲート
	UFUNCTION()
	void EndPlayMontage(FName NotifyName);
	//! @}
	
	//! @{@name 回転設定を更新
	void UpdateRotation();
	//! @} 
	
	float CalculateMaxSpeed() const ;
	
	void UpdateWarpTarget();
	
	void ExecPerformTraversalAction(const FTraversalCheckResult& TraversalCheckResult);
	
	TArray<UObject*> EvaluateTraversalAnimationChooser(FTraversalCheckResult TraversalCheckResult);
	
	bool PerformMotionMatch(TArray<UObject*> SearchAssets, FTraversalCheckResult& TraversalCheckResult);
	
#if WITH_EDITORONLY_DATA
	
	void DebugPrintTraversalResult(const FTraversalCheckResult& TraversalCheckResult);

	void DrawDebugShapesAtLedgeLocation(const FTraversalCheckResult& TraversalCheckResult);
	
#endif
	
	bool PerformForwardBlocks(FTraversalCheckResult& TraversalCheckResult, float TraceForwardDistance);
	
	bool PerformDecisionOnActorToEachEdge(FTraversalCheckResult& TraversalCheckResult);
	
	bool PerformActorToFrontEdge(FTraversalCheckResult& TraversalCheckResult, const FVector& HasRoomCheckFromLedgeLocation);
	
	void PerformObstacleDepth(FTraversalCheckResult& TraversalCheckResult, const FVector& HasRoomCheckFrontLedgeLocation, const FVector& HasRoomCheckBackLedgeLocation);
	
	void PerformBackLedgeFloor(FTraversalCheckResult& TraversalCheckResult, const FVector& HasRoomCheckBackLedgeLocation);
	
	void DetermineTraversalAction(FTraversalCheckResult& TraversalCheckResult);
	
	void SetInteractTransform(const FTraversalCheckResult& TraversalCheckResult);
	
	
	//!< 移動のスティック入力モード
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	TEnumAsByte<EAnalogueMovementBehavior> MovementStickMode = EAnalogueMovementBehavior::FixedSpeed_SingleStride;
	
	//!< カーブ
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	TObjectPtr<UCurveFloat> StrafeSpeedMapCurveObject = nullptr;	//!< アナログスティック入力閾値
	
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	float AnalogWalkRunThreshold = 0.7f;
	
	//!< 歩き状態の移動速度設定値
	//! @note X：最大 / Y : 中間 / Z : 最小
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	FVector WalkSpeed = FVector::ZeroVector;
	
	//!< 走り状態の移動速度設定値
	//! @note X：最大 / Y : 中間 / Z : 最小
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	FVector RunSpeed = FVector::ZeroVector;
	
	//!< スプリント状態の移動速度設定値
	//! @note X：最大 / Y : 中間 / Z : 最小
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	FVector SprintSpeed = FVector::ZeroVector;
	
	//!< 屈み状態の移動速度設定値
	//! @note X：最大 / Y : 中間 / Z : 最小
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	FVector CrouchSpeed = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "SN|Traversal")
	TObjectPtr<UChooserTable> TraversalAnimationChooser = nullptr;
	
	UPROPERTY()
	UPlayMontageCallbackProxy* MontageProxy = nullptr;
	
	//!< 歩幅設定
	TEnumAsByte<EStride> Stride = EStride::Run;
	
	//!< 歩き動作への移行フラグ
	bool bWantsToWalk = false;
	
	//!< スプリント動作への移行フラグ
	bool bWantsToSprint = false;
	
	//!< ストレイフモードへの移行フラグ
	bool bWantsToStrafe = true;
	
	//!< 視線を合わせる動作への移行フラグ
	bool bWantsToAim = false;
	
	//!< 直前に着地したかどうか
	bool bJustLanded = false;
	
	//!< 着地速度
	FVector LandSpeed = FVector::ZeroVector;
	
	//!< トラバーサルアクション判定の結果
	FTraversalCheckResult TraversalResult;
	
	bool bDoingTraversalAction = false;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	bool DrawDebugInfo = false;
	
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	int DrawDebugLevel = 0;
	
	UPROPERTY(EditAnywhere, Category = "SN|Traversal")
	float DrawDebugDuration = 10.0f;
	
#endif
};

FORCEINLINE EStride USNTraversalComponent::GetStride() const {
	return Stride;
}

FORCEINLINE bool USNTraversalComponent::GetWantsToSprint() const {
	return bWantsToSprint;
}

FORCEINLINE void USNTraversalComponent::SetWantsToSprint(bool Flag){
	bWantsToSprint = Flag;
}

FORCEINLINE bool USNTraversalComponent::GetWantsToWalk() const {
	return bWantsToWalk;
}

FORCEINLINE void USNTraversalComponent::SetWantsToWalk(bool Flag){
	bWantsToWalk = Flag;
}

FORCEINLINE bool USNTraversalComponent::GetWantsToStrafe() const {
	return bWantsToStrafe;
}

FORCEINLINE void USNTraversalComponent::SetWantsToStrafe(bool Flag){
	bWantsToStrafe = Flag;
}

FORCEINLINE bool USNTraversalComponent::GetWantsToAim() const {
	return bWantsToAim;
}
	
FORCEINLINE void USNTraversalComponent::SetWantsToAim(bool Flag){
	bWantsToAim = Flag;
}

FORCEINLINE bool USNTraversalComponent::IsJustLanded() const {
	return bJustLanded;
}

FORCEINLINE bool USNTraversalComponent::IsDoingTraversalAction() const {
	return bDoingTraversalAction;
}

FORCEINLINE const FVector& USNTraversalComponent::GetLandSpeed() const {
	return LandSpeed;
}
