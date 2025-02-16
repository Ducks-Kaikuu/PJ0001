// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/System/PJGameInstance.h"
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
}

UDamageData* UPJGameInstance::GetDamageData()
{
	return DamageData;
}
