// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_PlayRate.h"

void UPJAnimNotifyState_PlayRate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);

	if (AnimMontage == nullptr)
	{
		return;
	}

	if (PlayRateCurve != nullptr)
	{
		
	} else
	{
		AnimInstance->Montage_SetPlayRate(AnimMontage, PlayRate);
	}
}

void UPJAnimNotifyState_PlayRate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);

	if (AnimMontage == nullptr)
	{
		return;
	}
	
	AnimInstance->Montage_SetPlayRate(AnimMontage, 1.0f);
}
