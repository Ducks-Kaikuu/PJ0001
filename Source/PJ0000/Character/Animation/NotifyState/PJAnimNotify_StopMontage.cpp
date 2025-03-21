// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotify_StopMontage.h"

#include "GameFramework/Character.h"

void UPJAnimNotify_StopMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return;
	}

	AnimInstance->Montage_Stop(0.25f);
}
