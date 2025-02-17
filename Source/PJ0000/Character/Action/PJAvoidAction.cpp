// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Action/PJAvoidAction.h"

#include "Character/Base/SNCharacterBase.h"

bool UPJAvoidAction::ExecAction(const FInputActionValue& InputActionValue)
{
	bool Result = Super::ExecAction(InputActionValue);

	ASNCharacterBase* Character = GetOwner<ASNCharacterBase>();

	if (Character == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = Character->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return false;
	}

	UAnimMontage* AnimMontage = AnimInstance->GetCurrentActiveMontage();

	if (AnimMontage != nullptr)
	{
		AnimInstance->Montage_Stop(0.25f, AnimMontage);
	}

	AnimInstance->Montage_Play(Montage);
	
	return Result;
}
