// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/System/PJGameInstance.h"

#include "PJTimerManager.h"
#include "Character/NPC/PJEnemyManager.h"
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
			EnemyManager = NewObject<UPJEnemyManager>(UClass::StaticClass());
		}
	}

	if (TimerManagerClass.IsNull() == false)
	{
		UClass* Class = TimerManagerClass.LoadSynchronous();

		if (Class != nullptr)
		{
			TimerManager = NewObject<UPJTimerManager>(UClass::StaticClass());
		}
	}
}

UDamageData* UPJGameInstance::GetDamageData()
{
	return DamageData;
}
