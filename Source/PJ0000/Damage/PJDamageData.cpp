// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Damage/PJDamageData.h"

FDamageTable UDamageData::InvalidateData;

void UDamageData::Initialize()
{
	if (DamageDataTable.IsValid())
	{
		DamageData = DamageDataTable.LoadSynchronous();
	}
}

TArray<const FDamageTable*> UDamageData::GetDamageList(const FGameplayTagContainer& DamageTags)
{
	TArray<const FDamageTable*> DamageList;

	if (DamageData != nullptr)
	{
		TArray<FName> RowNames = DamageData->GetRowNames();

		for (auto& Name : RowNames)
		{
			const FDamageTable* Damage(DamageData->FindRow<FDamageTable>(Name, TEXT("Context")));

			if (DamageTags.HasTag(Damage->Tag))
			{
				DamageList.Add(Damage);
			}
		}
	}

	return DamageList;
}

const FDamageTable* UDamageData::GetDamage(const FGameplayTag& Tag) const
{
	if (DamageData == nullptr)
	{
		return nullptr;	
	}

	TArray<FName> RowNames = DamageData->GetRowNames();

	for (auto& Name : RowNames)
	{
		const FDamageTable* Damage(DamageData->FindRow<FDamageTable>(Name, TEXT("Context")));

		if (Damage->Tag == Tag)
		{
			return Damage;
		}
	}

	return nullptr;
}
