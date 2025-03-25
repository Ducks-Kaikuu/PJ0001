// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PJTimerManager.h"

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
