// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Animation/SNMotionMatchingAnimInstance.h"

#include "SNDef.h"

#include "AnimationWarpingLibrary.h"
#include "AnimExecutionContextLibrary.h"
#include "Chooser.h"
#include "ChooserFunctionLibrary.h"
#include "Animation/AnimSubsystem_Tag.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PoseSearch/MotionMatchingAnimNodeLibrary.h"
#include "PoseSearch/PoseSearchDatabase.h"

#include "Kismet/KismetMathLibrary.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

void USNMotionMatchingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if((OwnerCharacter == nullptr) || (OwnerMovementComponent == nullptr))
	{
		ASNMotionMatchingPlayerBase* Player(Cast<ASNMotionMatchingPlayerBase>(TryGetPawnOwner()));

		if(Player != nullptr)
		{
			OwnerCharacter = Player;

			OwnerMovementComponent = Player->GetCharacterMovement();
		}
	} else
	{
		UpdateEssentialValues();

		GenerateTrajectory();

		UpdateStates();
	}
}

void USNMotionMatchingAnimInstance::SetInteractTransform(const FTransform& Transform)
{
	InteractionTransform = Transform;
}

float USNMotionMatchingAnimInstance::GetTimeToLand() const
{
	return TrajectoryCollision.TimeToLand;
}

void USNMotionMatchingAnimInstance::UpdateEssentialValues()
{
		CharacterTransform = OwnerCharacter->GetActorTransform();
#if !UE_BUILD_SHIPPING
	// 可能であれば、オフセットされたルートボーンノードからルートボーントランスフォームをキャッシュします。
	// Get Socket Transform 関数を使用する代わりにこの方法でオフセットを保存すると、より正確な値が得られます。
	// スケルタルメッシュはエディタではデフォルトで -90 度回転されるため、他のトランスフォームとの角度比較を容易にするためにオフセットが追加されます。
	OffsetRootBoneEnable = UKismetSystemLibrary::GetConsoleVariableBoolValue(TEXT("a.animnode.offsetrootbone.enable"));
#else
	OffsetRootBoneEnable =  true;
#endif
	if(OffsetRootBoneEnable == true)
	{
		IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(GetClass());

		const FAnimSubsystem_Tag& AnimSubsystemTag = AnimClassInterface->GetSubsystem<FAnimSubsystem_Tag>();

		FAnimNode_OffsetRootBone* OffsetRootBone = AnimSubsystemTag.FindNodeByTag<FAnimNode_OffsetRootBone>(TEXT("OffsetRoot"), this);

		if(OffsetRootBone != nullptr)
		{
			FAnimNodeReference OffsetRootBoneReference(this, *OffsetRootBone);
		
			const TArray<FStructProperty*>& AnimNodeProperties = AnimClassInterface->GetAnimNodeProperties();
		
			FAnimNodeReference NodeReference = UAnimExecutionContextLibrary::GetAnimNodeReference(this, 0);

			FTransform OffsetRootBoneTransform = UAnimationWarpingLibrary::GetOffsetRootTransform(OffsetRootBoneReference);

			FRotator Tmp(OffsetRootBoneTransform.Rotator());

			Tmp.Yaw += 90.0f;

			RootTransform = UKismetMathLibrary::MakeTransform(OffsetRootBoneTransform.GetLocation(), Tmp, FVector::OneVector);
		}
	} else
	{
		RootTransform = CharacterTransform;
	}
	// キャラクタの加速度に関する重要な情報をキャッシュします。これは、物理的な加速度ではなく、移動コンポーネントによって適用される入力加速度です。
	// これらの値のすべてが現在グラフで使用されているわけではありませんが、追加機能のために手元に置いておくことは重要です。
	if(OwnerMovementComponent != nullptr)
	{
		Acceleration = OwnerMovementComponent->GetCurrentAcceleration();

		AccelerationAmount = Acceleration.Length() / OwnerMovementComponent->GetMaxAcceleration();
	}

	HasAcceleration = (AccelerationAmount > 0.0f) ? true : false;
	// キャラクタのベロシティに関する重要な情報をキャッシュします。
	// これらの値のすべてが現在グラフで使用されているわけではありませんが、追加機能のために手元に置いておくことは重要です。
	VelocityLastFrame = Velocity;
	
	if(OwnerMovementComponent != nullptr)
	{
		Velocity = OwnerMovementComponent->Velocity;
	}

	Speed2D = Velocity.Size2D();

	HasVelocity = (Speed2D > 5.0f) ? true : false;
	
	VelocityAcceleration = (Velocity - VelocityLastFrame) / FMath::Max(UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.001f);
	
	if(HasVelocity == true)
	{
		LastNonZeroVelocity = Velocity;
	}
	// 現在選択されているMotionマッチングデータベースのタグをキャッシュする。これにより、現在のデータベースに基づいて追加の選択を行うことができる。
	if(IsValid(CurrentSelectedDatabase))
	{
		CurrentDatabaseTags = CurrentSelectedDatabase->Tags;
	}
}

void USNMotionMatchingAnimInstance::GenerateTrajectory()
{
	FPoseSearchTrajectoryData& MoveTrajectoryData(TrajectoryGenerationDataMoving);
	
	const FPoseSearchTrajectoryData& TrajectoryData = (Speed2D > 0.0f) ? MoveTrajectoryData : TrajectoryGenerationDataIdle;

	FPoseSearchQueryTrajectory ResultTrajectory;
	
	UPoseSearchTrajectoryLibrary::PoseSearchGenerateTrajectory(this, TrajectoryData, GetDeltaSeconds(), Trajectory, PreviousDesiredControllerYaw, ResultTrajectory, -1.0f, 30, 0.1f, 15);

	UPoseSearchTrajectoryLibrary::HandleTrajectoryWorldCollisions(GetWorld(), this, ResultTrajectory, true, 0.01f, Trajectory, TrajectoryCollision, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, true, 150.0f);

	FPoseSearchQueryTrajectorySample Sample00;

	UPoseSearchTrajectoryLibrary::GetTrajectorySampleAtTime(Trajectory, 0.5f, Sample00);
	
	FPoseSearchQueryTrajectorySample Sample01;

	UPoseSearchTrajectoryLibrary::GetTrajectorySampleAtTime(Trajectory, 0.4f, Sample01);

	FVector TmpVector((Sample00.Position - Sample01.Position) * 10.0f);

	FutureVelocity = TmpVector;
}

void USNMotionMatchingAnimInstance::UpdateStates()
{
	MovementModeLastFrame = MovementMode;

	switch(OwnerMovementComponent->MovementMode)
	{
	case MOVE_None:
	case MOVE_Walking:
	case MOVE_NavWalking:
		MovementMode = EMotionMatchingMovementMode::OnGround; break;
	case MOVE_Falling:
	case MOVE_Flying:
		MovementMode = EMotionMatchingMovementMode::InAir; break;
	default:
		MovementMode = EMotionMatchingMovementMode::OnGround;
		//		SNPLUGIN_ERROR(TEXT("Invalid Movement Mode."));
		break;
	}
	
	RotationModeLastFrame = RotationMode;

	if(OwnerMovementComponent->bOrientRotationToMovement == true)
	{
		RotationMode = EMotionMatchingRotationMode::OrientToMovement;
	} else
	{
		RotationMode = EMotionMatchingRotationMode::Strafe;
	}

	MovementStateLastFrame = MovementState;

	if(IsMoving() == true)
	{
		MovementState = EMotionMatchingMovementState::Moving;
	} else
	{
		MovementState = EMotionMatchingMovementState::Idle;
	}
	
	
	// キャラクタのGaitをキャッシュし、最後のフレームの値を保存します。Gait Enumは、現在の速度ではなく、入力から意図された運動スタイルを記述します。
	// 例えば、キャラクタはゆっくり動いていても、Run Gait Enumの状態である可能性があります。
	StrideLastFrame = Stride;

	USNTraversalComponent* TraversalComponent(OwnerCharacter->GetTraversalComponent());
	
	Stride = TraversalComponent->GetStride();
	// キャラクタのスタンスをキャッシュし、最後のフレームの値を保存します。Stance Enumは、キャラクタの移動コンポーネントがしゃがみモードにあるかどうかを表します。
	StanceLastFrame = Stance;

	if(OwnerMovementComponent->IsCrouching() == true)
	{
		Stance = EMotionMatchingStance::Crouch;
	} else
	{
		Stance = EMotionMatchingStance::Stand;
	}
}

FVector USNMotionMatchingAnimInstance::CalculateRelativeAccelerationAmount() const
{
	FVector Result(FVector::ZeroVector);

	if(OwnerMovementComponent != nullptr)
	{
		if((OwnerMovementComponent->MaxAcceleration > 0.0f)
		&& (OwnerMovementComponent->GetMaxBrakingDeceleration() > 0.0f))
		{
			if(FVector::DotProduct(Acceleration, Velocity) > 0.0f)
			{
				FVector TmpVector(UKismetMathLibrary::Vector_ClampSizeMax(VelocityAcceleration, OwnerMovementComponent->MaxAcceleration));

				TmpVector /= OwnerMovementComponent->MaxAcceleration;

				Result = UKismetMathLibrary::Quat_UnrotateVector(CharacterTransform.GetRotation(), TmpVector);
			} else
			{
				FVector TmpVector(UKismetMathLibrary::Vector_ClampSizeMax(VelocityAcceleration, OwnerMovementComponent->GetMaxBrakingDeceleration()));

				TmpVector /= OwnerMovementComponent->GetMaxBrakingDeceleration();

				Result = UKismetMathLibrary::Quat_UnrotateVector(CharacterTransform.GetRotation(), TmpVector);
			}
		}
	}

	return Result;
}

FVector2D USNMotionMatchingAnimInstance::GetLeanAmount() const
{
	FVector RelativeAccelerate(CalculateRelativeAccelerationAmount());

	float Range = UKismetMathLibrary::MapRangeClamped(Speed2D, 200.0f, 500.0f, 0.5, 1.0f);
	
	float Result = RelativeAccelerate.Y * Range;

	return FVector2D(Result, 0.0f);
}

bool USNMotionMatchingAnimInstance::IsMoving() const
{
	if((UKismetMathLibrary::NotEqual_VectorVector(Velocity      , FVector::ZeroVector, 0.1f) == true)
	&& (UKismetMathLibrary::NotEqual_VectorVector(FutureVelocity, FVector::ZeroVector, 0.1f) == true))
	{
		return true;
	}
	
	return false;

}

bool USNMotionMatchingAnimInstance::IsStarting() const
{
	float FutureSpeed = FutureVelocity.Size2D();
	float Speed = Velocity.Size2D();

	if (IsMoving() == true 
	&& (FutureSpeed >= (Speed + 100.0f))
	&& (CurrentDatabaseTags.Contains(TEXT("Pivots")) == false))
	{
		return true;
	}
	
	return false;
}

bool USNMotionMatchingAnimInstance::IsPivoting() const
{
	FRotator FutureOrientation = UKismetMathLibrary::MakeRotFromX(FutureVelocity);
	FRotator Orientation = UKismetMathLibrary::MakeRotFromX(Velocity);

	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(FutureOrientation, Orientation);

	float Threshold = 0.0f;
	switch (RotationMode)
	{
	case EMotionMatchingRotationMode::OrientToMovement:
		Threshold = 60.0f;	break;
	case EMotionMatchingRotationMode::Strafe:
		Threshold = 40.0f;	break;
	default:
		break;
	}

	if (UKismetMathLibrary::Abs(Delta.Yaw) >= Threshold)
	{
		return true;
	}

	return false;

}

bool USNMotionMatchingAnimInstance::ShouldTurnInPlace() const
{
	if(OwnerCharacter == nullptr)
	{
		return false;
	}
	
	USNTraversalComponent* TraversalComponent = OwnerCharacter->GetTraversalComponent();
	
	if (TraversalComponent == nullptr)
	{
		SNPLUGIN_LOG(TEXT("AnimInst : MMLocomotionComponent is nullptr."));

		return false;
	}
	
	bool Result = false;
	// 止まった瞬間かチェック(MovementStateがIdleに変わった瞬間)
	bool bStopTrigger = ((MovementState == EMotionMatchingMovementState::Idle) && (MovementStateLastFrame == EMotionMatchingMovementState::Moving));
	
	if((bStopTrigger == true) || (TraversalComponent->GetWantsToAim() == true))
	{
		
		FRotator DeltaRotator(UKismetMathLibrary::NormalizedDeltaRotator(CharacterTransform.Rotator(), RootTransform.Rotator()));
		
		if(FMath::Abs(DeltaRotator.Yaw) >= 50.0f){
			Result = true;
		}
	}
	
	return Result;

}

bool USNMotionMatchingAnimInstance::JustLandedLight() const
{
	if(OwnerCharacter == nullptr)
	{
		return false;
	}
	USNTraversalComponent* TraversalComponent = OwnerCharacter->GetTraversalComponent();
	
	if (TraversalComponent == nullptr)
	{
		SNPLUGIN_LOG(TEXT("AnimInst : MMLocomotionComponent is nullptr."));

		return false;
	}

	
	if((TraversalComponent->IsJustLanded() == true)
	&& (FMath::Abs(TraversalComponent->GetLandSpeed().Z) < FMath::Abs(HeavyLandSpeedThreshold)))
	{
		return true;
	}

	return false;
}

bool USNMotionMatchingAnimInstance::JustLandedHeavy() const
{
	if(OwnerCharacter == nullptr)
	{
		return false;
	}
	USNTraversalComponent* TraversalComponent = OwnerCharacter->GetTraversalComponent();
	
	if (TraversalComponent == nullptr)
	{
		SNPLUGIN_LOG(TEXT("AnimInst : MMLocomotionComponent is nullptr."));

		return false;
	}
	
	if((TraversalComponent->IsJustLanded() == true)
	&& (FMath::Abs(TraversalComponent->GetLandSpeed().Z) >= FMath::Abs(HeavyLandSpeedThreshold)))
	{
		return true;
	}

	return false;
}

bool USNMotionMatchingAnimInstance::JustTraversed() const
{
	if((IsSlotActive(FName(TEXT("DefaultSlot"))) != true)
	&& (GetCurveValue(FName(TEXT("MovingTraversal"))) > 0.0))
	{
		return true;
	}
	
	return false;

}

EOffsetRootBoneMode USNMotionMatchingAnimInstance::GetOffsetRootRotationMode() const
{
	if(IsSlotActive(FName(TEXT("DefaultSlot"))))
	{
		return EOffsetRootBoneMode::Release;
	} else
	{
		return EOffsetRootBoneMode::Accumulate;
	}
}

EOffsetRootBoneMode USNMotionMatchingAnimInstance::GetOffsetRootTranslationMode() const
{
	if(IsSlotActive(FName(TEXT("DefaultSlot"))))
	{
		return EOffsetRootBoneMode::Release;
	} else
	{
		if(MovementMode == EMotionMatchingMovementMode::OnGround)
		{
			if(IsMoving() == true)
			{
				return EOffsetRootBoneMode::Interpolate;
			} else
			{
				return EOffsetRootBoneMode::Release;
			}
		}
	}

	return EOffsetRootBoneMode::Release;
}

float USNMotionMatchingAnimInstance::GetOffsetRootTranslationHalfLife() const
{
	if(MovementState == EMotionMatchingMovementState::Idle)
	{
		return 0.1f;
	} else
	{
		return 0.2f;
	}
}

EOrientationWarpingSpace USNMotionMatchingAnimInstance::GetOrientationWarpingSpace() const
{
	if(OffsetRootBoneEnable == true)
	{
		return EOrientationWarpingSpace::RootBoneTransform;
	} else
	{
		return EOrientationWarpingSpace::CustomTransform;
	}
}

bool USNMotionMatchingAnimInstance::EnableAO() const 
{
	FVector2D AOValue(GetAOValue());

	if((FMath::Abs(AOValue.X) <= 115.0f)
	&& (RotationMode == EMotionMatchingRotationMode::Strafe)
	&& (GetSlotMontageLocalWeight(FName(TEXT("DefaultSlot"))) < 0.5f))
	{
		return true;
	}

	return false;
}

FVector2D USNMotionMatchingAnimInstance::GetAOValue() const
{
	if(OwnerCharacter == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	FRotator CharacterRotate(OwnerCharacter->GetControlRotation());

	FRotator Normalized(UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotate, RootTransform.Rotator()));

	return FVector2D(Normalized.Yaw, Normalized.Pitch);
}

void USNMotionMatchingAnimInstance::UpdateMotionMatching(const FAnimUpdateContext& Context,	const FAnimNodeReference& Node)
{
	// ここからChooserTableのEvaluate処理
	// 2024年6月時点で実験的機能なのでこれから結構変更ありそう...。
	FChooserEvaluationContext ChooserContext(UChooserFunctionLibrary::MakeChooserEvaluationContext());

	ChooserContext.AddObjectParam(this);
	// 評価するChooserを設定
	FInstancedStruct ChooserInstance(UChooserFunctionLibrary::MakeEvaluateChooser(AnimationSearchChooser));
	// 評価を実行
	TArray<UObject*> Assets(UChooserFunctionLibrary::EvaluateObjectChooserBaseMulti(ChooserContext, ChooserInstance, UPoseSearchDatabase::StaticClass()));

	EAnimNodeReferenceConversionResult Result = EAnimNodeReferenceConversionResult::Failed;
	
	const FMotionMatchingAnimNodeReference& MotionMatchingNode = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(Node, Result);
	
	EPoseSearchInterruptMode InterruptMode = GetMotionMatchingInterruptMode();

	TArray<UPoseSearchDatabase*> DatabaseList;

	for(auto& Element:Assets)
	{
		UPoseSearchDatabase* Database = Cast<UPoseSearchDatabase>(Element);

		if(Database != nullptr)
		{
			DatabaseList.Add(Database);
		}
	}
		
	UMotionMatchingAnimNodeLibrary::SetDatabasesToSearch(MotionMatchingNode, DatabaseList, InterruptMode);
}

void USNMotionMatchingAnimInstance::UpdateMotionMatchingPostSelection(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result = EAnimNodeReferenceConversionResult::Failed;
	
	const FMotionMatchingAnimNodeReference& MotionMatchingNode = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(Node, Result);

	bool IsValidResult = false;
	
	FPoseSearchBlueprintResult SearchResult;
	
	UMotionMatchingAnimNodeLibrary::GetMotionMatchingSearchResult(MotionMatchingNode, SearchResult, IsValidResult);

	const UPoseSearchDatabase* Selected = SearchResult.SelectedDatabase.Get();
	
	CurrentSelectedDatabase = const_cast<UPoseSearchDatabase*>(Selected);
}

float USNMotionMatchingAnimInstance::GetMotionMatchingBlendTime() const
{
	float Result = 0.0f;

	if(MovementMode == EMotionMatchingMovementMode::OnGround)
	{
		if(MovementModeLastFrame == EMotionMatchingMovementMode::OnGround)
		{
			Result = 0.4f;
		} else
		{
			Result = 0.2f;
		}
	} else
	{
		if(Velocity.Z > 100.0f)
		{
			Result = 0.15f;
		} else
		{
			Result = 0.5f;
		}
	}
	
	return Result;
}

EPoseSearchInterruptMode USNMotionMatchingAnimInstance::GetMotionMatchingInterruptMode() const 
{
	bool bStateChanged = ((MovementState != MovementStateLastFrame)
	||(Stride != StrideLastFrame)
	||(Stance != StanceLastFrame));
	
	if((((bStateChanged == true) && (MovementMode == EMotionMatchingMovementMode::OnGround)))
		|| (MovementMode != MovementModeLastFrame))
	{
		return EPoseSearchInterruptMode::InterruptOnDatabaseChange;
	} else
	{
		return EPoseSearchInterruptMode::DoNotInterrupt;
	}
}

bool USNMotionMatchingAnimInstance::EnableSteering() const
{
	return ((MovementState == EMotionMatchingMovementState::Moving) || (MovementMode  == EMotionMatchingMovementMode::InAir));
}

FQuat USNMotionMatchingAnimInstance::GetDesiredFacing() const
{
	FPoseSearchQueryTrajectorySample TrajectorySample;
	
	UPoseSearchTrajectoryLibrary::GetTrajectorySampleAtTime(Trajectory, 0.5f, TrajectorySample);

	return TrajectorySample.Facing;
}
