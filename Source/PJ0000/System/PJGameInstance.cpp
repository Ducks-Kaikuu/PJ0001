// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/System/PJGameInstance.h"

#include "PJTimerManager.h"
#include "AI/EQS/PJEqsManager.h"
#include "Character/Enemy/PJEnemyManager.h"
#include "PJ0000/Damage/PJDamageData.h"

void UPJGameInstance::Init()
{
	Super::Init();

	if (DamageDataObject.IsNull() == false)
	{
		DamageData = DamageDataObject.LoadSynchronous();

		if (DamageData != nullptr)
		{
			DamageData->Initialize();
		}
	}

	if (EnemyManagerClass.IsNull() == false)
	{
		UClass* Class = EnemyManagerClass.LoadSynchronous();

		if (Class != nullptr)
		{
			EnemyManager = NewObject<UPJEnemyManager>(this, Class);
		}
	}

	if (TimerManagerClass.IsNull() == false)
	{
		UClass* Class = TimerManagerClass.LoadSynchronous();

		if (Class != nullptr)
		{
			TimerManager = NewObject<UPJTimerManager>(this, Class);
		}
	}

	if (EqsManagerClass.IsNull() == false)
	{
		UClass* Class = EqsManagerClass.LoadSynchronous();

		if (Class != nullptr)
		{
			EqsManager = NewObject<UPJEqsManager>(this, Class);

			if (EqsManager != nullptr)
			{
				EqsManager->Initialize();
			}
		}
	}
}

