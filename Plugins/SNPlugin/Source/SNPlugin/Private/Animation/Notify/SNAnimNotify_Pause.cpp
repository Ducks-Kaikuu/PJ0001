// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotify_Pause.h"

void USNAnimNotify_Pause::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);

	if (Montage != nullptr)
	{
		UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Pause(Montage);
		}
	}
}
