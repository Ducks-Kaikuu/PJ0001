// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_Vanish.h"

#include "STransformViewportToolbar.h"
#include "Character/Base/SNCharacterBase.h"

void UPJAnimNotifyState_Vanish::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(MeshComp->GetOwner());

	if (Character != nullptr)
	{
		FVector Position = Character->GetActorLocation() + Character->GetActorForwardVector() * -1 * Speed;

		Character->SetActorLocation(Position);
	}
}
