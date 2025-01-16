// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SNAnimInstanceBase.h"
#include "MotionMatching/SNMotionMatchingDef.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "BoneControllers/AnimNode_OffsetRootBone.h"
#include "BoneControllers/AnimNode_OrientationWarping.h"
#include "SNMotionMatchingAnimInstance.generated.h"

class ASNMotionMatchingPlayerBase;
class UCharacterMovementComponent;
class UChooserTable;
/**
 * 
 */
UCLASS()
class SNANIMATION_API USNMotionMatchingAnimInstance : public USNAnimInstanceBase
{
	GENERATED_BODY()

public:
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	//! @{@name インタラクトしたオブジェクトのトランスフォームを設定
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual void SetInteractTransform(const FTransform& Transform);
	//! @}
	
	//! @{@name インタラクトしたオブジェクトのトランスフォームを取得
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual const FTransform& GetInteractTransform() const ;
	//! @}
	
	// AnimGraphから参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|MotionMatching|AdditiveLean", meta=(BlueprintThreadSafe))
	FVector2D GetLeanAmount() const ;
	
	//! @{@name プレイヤーが移動中かチェック - AnimGraphから参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool IsMoving() const ;
	//! @}
	
	//! @{@name 移動を開始したタイミングかチェック - Chooser内で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool IsStarting() const ;
	//! @}
	
	// Chooser内で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool IsPivoting() const ;
	
	// Chooser内で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool ShouldTurnInPlace() const ;
	
	// Chooser内で参照	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool JustLandedLight() const ;
	
	// Chooser内で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool JustLandedHeavy() const ;
	
	// Chooser内で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|Traversal|Movement Analysis", meta=(BlueprintThreadSafe))
	bool JustTraversed() const ;
	
	// AnimGraphから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|RootOffset", meta=(BlueprintThreadSafe))
	EOffsetRootBoneMode GetOffsetRootRotationMode() const ;
	
	// AnimGraphから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|RootOffset", meta=(BlueprintThreadSafe))
	EOffsetRootBoneMode GetOffsetRootTranslationMode() const ;
	
	// AnimGraphから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|RootOffset", meta=(BlueprintThreadSafe))
	float GetOffsetRootTranslationHalfLife() const ;
	
	// AnimGraphから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|RootOffset", meta=(BlueprintThreadSafe))
	EOrientationWarpingSpace GetOrientationWarpingSpace() const ;
	
	// AnimGraph - BS_Neutral_AO_Standから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|AimOffset", meta=(BlueprintThreadSafe))
	bool EnableAO() const ;
	
	// AnimGraph - BS_Neutral_AO_Standから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|AimOffset", meta=(BlueprintThreadSafe))
	FVector2D GetAOValue() const ;
	
	// AnimGraphのMotion Matchingノードから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|MotionMatching", meta=(BlueprintThreadSafe))
	void UpdateMotionMatching(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	// AnimGraphのMotion Matchingノードから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|MotionMatching", meta=(BlueprintThreadSafe))
	void UpdateMotionMatchingPostSelection(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	
	// AnimGraphのMotion Matchingノードから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|MotionMatching", meta=(BlueprintThreadSafe))
	float GetMotionMatchingBlendTime() const ;
	
	// AnimationBlendStackGraph_0 - Steeringノードから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|Steering", meta=(BlueprintThreadSafe))
	bool EnableSteering() const ;
	
	// AnimationBlendStackGraph_0 - Steeringノードから参照
	UFUNCTION(BlueprintCallable, Category="SN|Traversal|Steering", meta=(BlueprintThreadSafe))
	FQuat GetDesiredFacing() const ;
	
	// AnimationBlendStackGraph_0で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|MotionMatching", meta=(BlueprintThreadSafe))
	const UPoseSearchDatabase* GetCurrentSelectedDatabase() const ;
	
	// AnimationBlendStackGraph_0で参照
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SN|MotionMatching", meta=(BlueprintThreadSafe))
	const FVector& GetLastNonZeroVelocity() const ;

	// Chooser内で参照
	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching|State", meta=(BlueprintThreadSafe))
	TEnumAsByte<EMotionMatchingMovementMode> GetMovementMode() const ;

	// Chooser内で参照
	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching|State", meta=(BlueprintThreadSafe))
	TEnumAsByte<EMotionMatchingMovementState> GetMovementState() const ;

	// Chooser内で参照
	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching|State", meta=(BlueprintThreadSafe))
	TEnumAsByte<EMotionMatchingStance> GetStance() const ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SN|MotionMatching")
	TEnumAsByte<EStride> Stride = EStride::Walk;

	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching|State", meta=(BlueprintThreadSafe))
	TEnumAsByte<EStride> GetStride() const ;

	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching", meta=(BlueprintThreadSafe))
	float GetSpeed2D() const ;

	UFUNCTION(BlueprintCallable, Category="SN|MotionMatching", meta=(BlueprintThreadSafe))
	float GetTimeToLand() const ;

private:
	
	void UpdateEssentialValues();
	
	void GenerateTrajectory();
	
	void UpdateStates();
	
	FVector CalculateRelativeAccelerationAmount() const ;
	
	EPoseSearchInterruptMode GetMotionMatchingInterruptMode() const ;
	
	FTransform InteractionTransform;
	
	UPROPERTY()
	ASNMotionMatchingPlayerBase* OwnerCharacter = nullptr;
	
	UPROPERTY()
	UCharacterMovementComponent* OwnerMovementComponent = nullptr;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	TEnumAsByte<EMotionMatchingMovementMode> MovementMode= EMotionMatchingMovementMode::OnGround;
	
	TEnumAsByte<EMotionMatchingMovementMode> MovementModeLastFrame = EMotionMatchingMovementMode::OnGround;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	TEnumAsByte<EMotionMatchingRotationMode> RotationMode = EMotionMatchingRotationMode::OrientToMovement;
	
	TEnumAsByte<EMotionMatchingRotationMode> RotationModeLastFrame = EMotionMatchingRotationMode::OrientToMovement;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	TEnumAsByte<EMotionMatchingMovementState> MovementState = EMotionMatchingMovementState::Idle;
	
	TEnumAsByte<EMotionMatchingMovementState> MovementStateLastFrame = EMotionMatchingMovementState::Idle;
	
	// Chooser内で参照
	
	TEnumAsByte<EStride> StrideLastFrame = EStride::Walk;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	TEnumAsByte<EMotionMatchingStance> Stance = EMotionMatchingStance::Stand;
	
	TEnumAsByte<EMotionMatchingStance> StanceLastFrame = EMotionMatchingStance::Stand;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	int MMDatabaseLOD = 0;
	
	// Chooser内で参照
	UPROPERTY(EditAnywhere, Category = "SN|MotionMatching")
	float Speed2D = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Essential")
	TArray<FName> CurrentDatabaseTags;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Essential")
	float HeavyLandSpeedThreshold = 700.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Essential")
	bool OffsetRootBoneEnable = false;
	
	UPROPERTY(EditAnywhere, Category="SN|MotionMatching|Animation Chooser")
	TObjectPtr<UChooserTable> AnimationSearchChooser = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FPoseSearchTrajectoryData TrajectoryGenerationDataIdle;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FPoseSearchTrajectoryData TrajectoryGenerationDataMoving;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FPoseSearchTrajectoryData TrajectoryGenerationDataClimbing;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FPoseSearchQueryTrajectory Trajectory;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	FVector FutureVelocity = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, Category="SN|Traversal|Trajectory")
	float PreviousDesiredControllerYaw = 0.0f;
	
	UPROPERTY()
	TObjectPtr<UPoseSearchDatabase> CurrentSelectedDatabase = nullptr;
	
	FTransform CharacterTransform = FTransform::Identity;
	
	FTransform RootTransform = FTransform::Identity;
	
	FVector Acceleration = FVector::ZeroVector;
	
	FVector Velocity = FVector::ZeroVector;
	
	FVector VelocityLastFrame = FVector::ZeroVector;
	
	FVector VelocityAcceleration = FVector::ZeroVector;
	
	FVector LastNonZeroVelocity = FVector::ZeroVector;
	
	float AccelerationAmount = 0.0f;
	
	bool HasAcceleration= false;
	
	bool HasVelocity	= false;
};

//----------------------------------------------------------------------//
//
//! @brief インタラクトしたオブジェクトのトランスフォームを取得
//
//! @retval インタラクトしたオブジェクトのトランスフォーム
//
//----------------------------------------------------------------------//
FORCEINLINE const FTransform& USNMotionMatchingAnimInstance::GetInteractTransform() const {
	return InteractionTransform;
}

//----------------------------------------------------------------------//
//
//! @brief 現在選択されているデータベースへのポインタを取得
//
//! @retval 現在選択されているデータベースへのポインタ
//
//----------------------------------------------------------------------//
FORCEINLINE const UPoseSearchDatabase* USNMotionMatchingAnimInstance::GetCurrentSelectedDatabase() const {
	return CurrentSelectedDatabase;
}

FORCEINLINE const FVector& USNMotionMatchingAnimInstance::GetLastNonZeroVelocity() const {
	return LastNonZeroVelocity;
}

FORCEINLINE TEnumAsByte<EMotionMatchingMovementMode> USNMotionMatchingAnimInstance::GetMovementMode() const
{
	return MovementMode;
}

FORCEINLINE TEnumAsByte<EMotionMatchingMovementState> USNMotionMatchingAnimInstance::GetMovementState() const
{
	return MovementState;
}

FORCEINLINE TEnumAsByte<EMotionMatchingStance> USNMotionMatchingAnimInstance::GetStance() const
{
	return Stance;
}

FORCEINLINE TEnumAsByte<EStride> USNMotionMatchingAnimInstance::GetStride() const
{
	return Stride;
}

FORCEINLINE float USNMotionMatchingAnimInstance::GetSpeed2D() const
{
	return Speed2D;
} 