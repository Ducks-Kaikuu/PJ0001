// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotify_Death.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "Character/Components/SNDamageWithChooserComponent.h"
#include "GameFramework/Character.h"

void UPJAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character != nullptr)
	{
		USNDamageWithChooserComponent* DamageComponent = Character->FindComponentByClass<USNDamageWithChooserComponent>();

		if (DamageComponent != nullptr)
		{
			DamageComponent->Death();
		}
	}
}
