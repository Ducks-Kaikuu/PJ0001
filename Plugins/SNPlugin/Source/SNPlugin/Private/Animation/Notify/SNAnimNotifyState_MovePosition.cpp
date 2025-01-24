// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotifyState_MovePosition.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "Types/AttributeStorage.h"

void USNAnimNotifyState_MovePosition::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference){
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));
	
	if(Character == nullptr){
		return;
	}

	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));

	if (MovePositionComponent != nullptr)
	{
		MovePositionComponent->AddTransform(TransformName, Character->GetActorTransform(), EventReference.GetCurrentAnimationTime());
	}
}

void USNAnimNotifyState_MovePosition::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));

	if (Character == nullptr)
	{
		return;
	}
	
	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));

	if (MovePositionComponent != nullptr)
	{
		const FTransformData* TransformData = MovePositionComponent->GetTransformData(TransformName);

		if(TransformData != nullptr)
		{
			float ElapsedTime = EventReference.GetCurrentAnimationTime();
			
			float EndTime = TransformData->StartTime + (EventReference.GetNotify() != nullptr) ? EventReference.GetNotify()->GetDuration() : 0.0f;

			float DeltaRatio = CalculateNotifyStateProgressPercentage(TransformData->StartTime, EndTime, ElapsedTime);
			
			FVector Location(FVector::ZeroVector), TmpLocation(FVector::ZeroVector);
			
			if(bMoveLocation == true)
			{
				FVector CurveValue = FVector::ZeroVector;

				if (TranslateInterpolate != nullptr)
				{
					CurveValue = TranslateInterpolate->GetVectorValue(DeltaRatio);
				}

				if (bUseCurveAsAbsoluteValue == true)
				{
					TmpLocation = CurveValue;
				} else
				{
					TmpLocation.X = TargetLocation.X * CurveValue.X;
					TmpLocation.Y = TargetLocation.Y * CurveValue.Y;
					TmpLocation.Z = TargetLocation.Z * CurveValue.Z;				
				}

				if (bAdditive == false)
				{
					Location = TmpLocation;
				} else
				{
					Location = TransformData->StartTransform.GetLocation() + TmpLocation;
				}
			}

			FRotator Rotator = FRotator::ZeroRotator;
		
			if (RotateInterpolate != nullptr)
			{
				FVector Value(RotateInterpolate->GetVectorValue(DeltaRatio));
			
				Rotator.Pitch = Value.X;
				Rotator.Yaw = Value.Y;
				Rotator.Roll = Value.Z;
			}

			FTransform Transform(FRotator(Rotator), Location);

			Character->SetActorTransform(Transform);
		}
#if 0
		FTransform SrcTransform = MovePositionComponent->GetSrcTransform(TransformName);
		FTransform DestTransform = MovePositionComponent->GetDestTransform(TransformName);

		float LocationX = FMath::Lerp(DestTransform.GetLocation().X, SrcTransform.GetLocation().X, PosAlphaX);
		float LocationY = FMath::Lerp(DestTransform.GetLocation().Y, SrcTransform.GetLocation().Y, PosAlphaX);
		float LocationZ = FMath::Lerp(DestTransform.GetLocation().Z, SrcTransform.GetLocation().Z, PosAlphaX);

		float RotateX = FMath::Lerp(DestTransform.GetRotation().Rotator().Pitch, SrcTransform.GetRotation().Rotator().Pitch, RotAlphaX);
		float RotateY = FMath::Lerp(DestTransform.GetRotation().Rotator().Yaw, SrcTransform.GetRotation().Rotator().Yaw, RotAlphaY);
		float RotateZ = FMath::Lerp(DestTransform.GetRotation().Rotator().Roll, SrcTransform.GetRotation().Rotator().Roll, RotAlphaZ);

		FRotator Rotator(RotateX, RotateY, RotateZ);
		
		FTransform Transform(FRotator(Rotator), FVector(LocationX, LocationY, LocationZ));

		Character->SetActorTransform(Transform);
#endif
	}
}

float USNAnimNotifyState_MovePosition::CalculateNotifyStateProgressPercentage(float StartTime, float EndTime, float CurrentTime)
{
	// NotifyStateの総時間（終了時間 - 開始時間）
	float TotalDuration = EndTime - StartTime;
	// NotifyStateの開始時間からの経過時間
	float ElapsedTime = CurrentTime - StartTime;
	// パーセンテージを計算（0 ～ 1）
	float Percentage = (TotalDuration > 0) ? (ElapsedTime / TotalDuration) : 0.0f;

	float Result = FMath::Clamp(Percentage, 0.0f, 1.0f);
	// 
	SNPLUGIN_LOG(TEXT("StartTime: %f / EndTime : %f / CurrentTime : %f / Result : %f"), StartTime, EndTime, CurrentTime, Result);
	// クラッピングして0%～100%の範囲内に保つ
	return Result;
}
