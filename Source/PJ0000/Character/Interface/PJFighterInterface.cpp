// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Interface/PJFighterInterface.h"

#include "SNDef.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"

class UPJHealthSet;
// Add default functionality here for any IPJFighterInterface functions that are not pure virtual.
bool IPJFighterInterface::IsDead() const
{
	const ASNCharacterBase* Character(Cast<ASNCharacterBase>(this));

	if (Character == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("The class is not derived from ASNCharacterBase."));

		return false;
	}
	
	const UPJHealthSet* HealthSet = Character->GetGameAttribute<UPJHealthSet>();
	
	if (HealthSet != nullptr)
	{
		int HP = HealthSet->GetHealth();

		return (HP <= 0) ? true : false;
	}

	return false;

}
