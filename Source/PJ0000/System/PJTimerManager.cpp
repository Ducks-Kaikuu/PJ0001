// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PJTimerManager.h"

#include "NiagaraComponent.h"
#include "SNDef.h"
#include "Character/Base/SNPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/SNUtility.h"

void UPJTimerManager::UpdateTimer()
{
	

	APlayerController* PlayerController = SNUtility::GetPlayerController<APlayerController>();

	if (PlayerController != nullptr)
	{
		UGameplayStatics::SetGlobalTimeDilation(PlayerController->GetWorld(), WorldTimerRate);
		
		ASNPlayerBase* Player = Cast<ASNPlayerBase>(PlayerController->GetPawn());

		Player->CustomTimeDilation = PlayerSpeedupRate * PlayerTimerRate;

		SNPLUGIN_LOG(TEXT("Player Speed : %f - %f"), Player->CustomTimeDilation, WorldTimerRate);

		for (UNiagaraComponent* Component : IgnoreTimeRateVFX)
		{
			if (Component != nullptr)
			{
				Component->SetCustomTimeDilation(PlayerTimerRate);
			}
		}
	}	
	
}

void UPJTimerManager::SetWorldTimerRate(float Rate)
{
	WorldTimerRate = Rate;
	// プレイヤーがワールドの速度に影響を受けないための変数
	PlayerTimerRate = 1.0f / Rate;
}

void UPJTimerManager::SetPlayerSpeedupRate(float Rate)
{
	PlayerSpeedupRate = Rate;
}

void UPJTimerManager::SetIgnoreTimeRate(AActor* Actor, bool bIgnore)
{
	if (Actor != nullptr)
	{
		if (bIgnore == true)
		{
			Actor->CustomTimeDilation = PlayerTimerRate;
		} else
		{
			Actor->CustomTimeDilation = 1.0f;
		}
	}
}

void UPJTimerManager::SetNiagaraIgnoreTimeRate(UNiagaraComponent* Component, bool bIgnore)
{
	if (Component != nullptr)
	{
		if (bIgnore == true)
		{
			Component->SetCustomTimeDilation(PlayerTimerRate);
		} else
		{
			Component->SetCustomTimeDilation(1.0f);
		}
	}
}

void UPJTimerManager::AddIgnoreTimeRareVfx(UNiagaraComponent* Component)
{
	if (IgnoreTimeRateVFX.Contains(Component) == false)
	{
		IgnoreTimeRateVFX.Add(Component);

		Component->OnSystemFinished.AddDynamic(this, &UPJTimerManager::NiagaraSystemFinished);
	}
}

void UPJTimerManager::NiagaraSystemFinished(class UNiagaraComponent* Component)
{
	IgnoreTimeRateVFX.Remove(Component);

	Component->OnSystemFinished.RemoveAll(this);
}
