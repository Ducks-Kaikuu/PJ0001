// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_Vanish.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Base/SNPlayerBase.h"
#include "Utility/SNUtility.h"

void UPJAnimNotifyState_Vanish::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(MeshComp->GetOwner());

	APlayerController* PlayerController = SNUtility::GetPlayerController<APlayerController>();

	ASNPlayerBase* Player = nullptr;

	if (PlayerController != nullptr)
	{
		Player = Cast<ASNPlayerBase>(PlayerController->GetPawn());
	}

	if (Character != nullptr)
	{
		if (Player != nullptr)
		{
			if (bSelfOrient == false)
			{
				FVector Position = Character->GetActorLocation() + Player->GetActorForwardVector() * Speed;

				Character->SetActorLocation(Position);
			} else
			{
				FVector Position = Character->GetActorLocation() + Character->GetActorForwardVector() * -1 * Speed;

				Character->SetActorLocation(Position);
			}
			
		} else
		{
			FVector Position = Character->GetActorLocation() + Character->GetActorForwardVector() * -1 * Speed;

			Character->SetActorLocation(Position);
		}
	}
}
