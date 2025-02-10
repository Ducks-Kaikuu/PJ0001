// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Damage/PJDamageData.h"

void UDamageData::Initialize()
{
	if (DamageDataTable.IsNull() == false)
	{
		DamageData = DamageDataTable.LoadSynchronous();
	}
}

TArray<const FDamageTable*> UDamageData::GetDamageList(const FGameplayTagContainer& DamageTags)
{
	TArray<const FDamageTable*> DamageList;
	
	if (DamageData != nullptr)
	{
		DamageData->ForeachRow(TEXT(""), TFunctionRef<void(const FName&, const FDamageTable&)>(
			[&](const FName& Name, const FDamageTable& Damage)
			{
				if (DamageTags.HasAny(Damage.Tags))
				{
					DamageList.Add(&Damage);
				}
			}
		));
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

		if (Damage->Tags.HasTag(Tag) == true)
		{
			return Damage;
		}
	}

	return nullptr;
}
