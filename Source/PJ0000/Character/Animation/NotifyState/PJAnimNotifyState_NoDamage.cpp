// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_NoDamage.h"

#include "Character/PJCharacter.h"

void UPJAnimNotifyState_NoDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	APJCharacter* Player = Cast<APJCharacter>(MeshComp->GetOwner());

	if (Player == nullptr)
	{
		return;
	}

	Player->SetNoDamage(true);
}

void UPJAnimNotifyState_NoDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	APJCharacter* Player = Cast<APJCharacter>(MeshComp->GetOwner());

	if (Player == nullptr)
	{
		return;
	}

	Player->SetNoDamage(false);
}
