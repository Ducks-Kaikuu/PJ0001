// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotify_RemoveActionTag.h"

#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"

void USNAnimNotify_RemoveActionTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(MeshComp->GetOwner()));

	if (Character != nullptr)
	{
		Character->RemoveActionTagContainer(Tags);

		SNPLUGIN_LOG(TEXT("Remove Action Tag Container."))
	}
}
