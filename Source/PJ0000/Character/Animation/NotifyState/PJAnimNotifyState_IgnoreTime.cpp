// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/NotifyState/PJAnimNotifyState_IgnoreTime.h"

#include "Character/Base/SNPlayerBase.h"
#include "System/PJGameInstance.h"
#include "System/PJTimerManager.h"
#include "Utility/SNUtility.h"

void UPJAnimNotifyState_IgnoreTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ASNPlayerBase* Player = Cast<ASNPlayerBase>(MeshComp->GetOwner());

	if (Player != nullptr)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character != nullptr)
	{
		UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

		if (GameInstance != nullptr)
		{
			UPJTimerManager* TimerManager = GameInstance->GetTimerManager();

			if (TimerManager != nullptr)
			{
				TimerManager->SetIgnoreTimeRate(Character, true);
			}
		}
	}
}

void UPJAnimNotifyState_IgnoreTime::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ASNPlayerBase* Player = Cast<ASNPlayerBase>(MeshComp->GetOwner());

	if (Player != nullptr)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character != nullptr)
	{
		UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

		if (GameInstance != nullptr)
		{
			UPJTimerManager* TimerManager = GameInstance->GetTimerManager();

			if (TimerManager != nullptr)
			{
				TimerManager->SetIgnoreTimeRate(Character, false);
			}
		}
	}
}
