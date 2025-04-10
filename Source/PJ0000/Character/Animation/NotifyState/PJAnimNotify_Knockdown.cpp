// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotify_Knockdown.h"

#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPJAnimNotify_Knockdown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Pause();
	}

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(MeshComp->GetOwner());

	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->GravityScale = FallSpeed;

	}

	SNPLUGIN_LOG(TEXT("Strike is coming."));
}
