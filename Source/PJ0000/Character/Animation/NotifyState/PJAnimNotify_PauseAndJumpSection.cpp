// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotify_PauseAndJumpSection.h"

#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"

class UPJDamageWithChooserComponent;

void UPJAnimNotify_PauseAndJumpSection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(MeshComp->GetOwner());

	if (Character == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Pause();
	}

	UPJDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<UPJDamageWithChooserComponent>();

	if (DamageComponent == nullptr)
	{
		return;
	}

	if (LoopTime > 0.0f)
	{
		DamageComponent->StartResumeTimer(LoopTime, NextSectionName);
	}
}
