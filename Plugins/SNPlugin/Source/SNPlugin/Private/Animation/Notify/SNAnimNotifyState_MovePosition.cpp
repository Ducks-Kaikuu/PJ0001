// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotifyState_MovePosition.h"

#include "Animation/Components/SNMovePositionComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "Types/AttributeStorage.h"

void USNAnimNotifyState_MovePosition::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));

	if (Character == nullptr)
	{
		return;
	}
	
	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));

	if (MovePositionComponent != nullptr)
	{
		MovePositionComponent->SetStateTotalDuration(TransformName, TotalDuration);
		
		MovePositionComponent->AddSourceTransform(TransformName, Character->GetActorTransform());
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
		float DeltaRatio = FrameDeltaTime / MovePositionComponent->GetStateTotalDuration(TransformName);

		float PosAlphaX = DeltaRatio, PosAlphaY = DeltaRatio, PosAlphaZ = DeltaRatio;
		
		if (TranslateInterpolate != nullptr)
		{
			FVector Value(TranslateInterpolate->GetVectorValue(DeltaRatio));

			PosAlphaX = Value.X;
			PosAlphaY = Value.Y;
			PosAlphaZ = Value.Z;
		}

		float RotAlphaX(DeltaRatio), RotAlphaY(DeltaRatio), RotAlphaZ(DeltaRatio);

		if (RotateInterpolate != nullptr)
		{
			FVector Value(RotateInterpolate->GetVectorValue(DeltaRatio));

			RotAlphaX = Value.X;
			RotAlphaY = Value.Y;
			RotAlphaZ = Value.Z;
		}

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
	}
}
