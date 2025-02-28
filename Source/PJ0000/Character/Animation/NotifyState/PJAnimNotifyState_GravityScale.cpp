// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Animation/NotifyState/PJAnimNotifyState_GravityScale.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "AITypes.h"
#include "SNDef.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPJAnimNotifyState_GravityScale::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));

	if (Character != nullptr)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

		if (MovementComponent != nullptr)
		{
			OldGravityScale = MovementComponent->GravityScale;
			
			MovementComponent->GravityScale = GravityScale;

			MovementComponent->Velocity *= VelocityScale;

			SNPLUGIN_LOG(TEXT("GravityScale : %f"), GravityScale);
		}
	}
}

void UPJAnimNotifyState_GravityScale::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (bRestore == true)
	{
		ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));

		if (Character != nullptr)
		{
			UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

			if (MovementComponent != nullptr)
			{
				MovementComponent->GravityScale = OldGravityScale;

				SNPLUGIN_LOG(TEXT("GravityScale : %f"), OldGravityScale);
			}
		}
	}
}
