// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJUpperAttackAction.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "GameFramework/Character.h"
#include "PJ0000/Character/Components/SNComboComponent.h"

bool UPJUpperAttackAction::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNCharacterBase* Character(GetOwner<ASNCharacterBase>());

	if (Character != nullptr)
	{
		USNComboComponent* ComboComponent = Character->FindComponentByClass<USNComboComponent>();

		SNPLUGIN_LOG(TEXT("Upper Attack Combo : %f"), ComboComponent->GetComboScore());
		
		if ((ComboComponent != nullptr) && (PlayMontage == nullptr))
		{
			ComboComponent->SetComboScore(5.0f);

			PlayMontage = ComboComponent->PerformMotionMatching(Character);

			if (PlayMontage != nullptr)
			{
				PlayMontage->OnCompleted.AddDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);
				PlayMontage->OnInterrupted.AddDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);
				PlayMontage->OnBlendOut.AddDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);

				return true;
			} else {
				SNPLUGIN_LOG(TEXT("--------- Can't Find Motion ----------"));
			}
		}
	}

	return false;
}

void UPJUpperAttackAction::OnMontagePlayEnd(FName NotifyName)
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner<ACharacter>()));

	if (Character != nullptr)
	{
		Character->RemoveActionTag(FGameplayTag::RequestGameplayTag(TEXT("Input.Attack.Upper"), true));
		
		USNComboComponent* ComboComponent = Character->FindComponentByClass<USNComboComponent>();

		if (ComboComponent != nullptr)
		{
			ComboComponent->SetComboScore(0.0f);

			if (PlayMontage != nullptr)
			{
				PlayMontage->OnCompleted.RemoveDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);
				PlayMontage->OnInterrupted.RemoveDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);
				PlayMontage->OnBlendOut.RemoveDynamic(this, &UPJUpperAttackAction::OnMontagePlayEnd);

				PlayMontage = nullptr;
				
				SNPLUGIN_LOG(TEXT("Remove Dynamic"));
			}
		}
	}
}
