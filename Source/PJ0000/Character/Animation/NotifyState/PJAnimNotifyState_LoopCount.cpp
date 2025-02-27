// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_LoopCount.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
#include "GameFramework/Character.h"

void UPJAnimNotifyState_LoopCount::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(MeshComp->GetOwner());

	if (Character == nullptr)
	{
		return;
	}

	UPJDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<UPJDamageWithChooserComponent>();

	if (DamageComponent == nullptr)
	{
		return;
	}

	DamageComponent->AddLoopCount(1);

	if (DamageComponent->IsLoopEnd(LoopNum))
	{
		FGameplayTagContainer ActionTags = Character->GetActionTags();
		
		DamageComponent->ResetLoopCount();

		UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			const UAnimMontage* AnimMontage = AnimInstance->GetCurrentActiveMontage();
			
			AnimInstance->Montage_JumpToSection(NextSectionName, AnimMontage);
		}
	}
}
