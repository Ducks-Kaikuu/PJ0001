// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/PJDamageWithChooserComponent.h"

#include "SNDef.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Scene/SNSceneBase.h"
#include "UI/Widget/PJDamageWidget.h"
#include "UI/Widget/SNMasterWidget.h"
#include "Utility/SNUtility.h"

void UPJDamageWithChooserComponent::DrawDamage(int Damage)
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("PJDamageWithChooserComponent::DrawDamage - Character is nullptr."));

		return;
	}
	
	USNGameInstance* GameInstance = SNUtility::GetGameInstance<USNGameInstance>();
	
	UPJDamageWidget* DamageWidgetInstance = CreateWidget<UPJDamageWidget>(GameInstance, DamageWidget, TEXT("Damage"));

	if (DamageWidgetInstance != nullptr)
	{
		ASNSceneBase* Scene(GameInstance->GetCurrentScene());

		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());

		if ((Scene != nullptr) && (PlayerController != nullptr))
		{
			USNMasterWidget* MasterWidget = Scene->GetMasterWidget();

			if (MasterWidget != nullptr)
			{
				FVector WorldPosition(Character->GetActorLocation());

				//WorldPosition.Z += 150.0f;

				FVector2D ScreenPosition = FVector2D::ZeroVector;

				PlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition);

				ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(GetWorld());
				
				MasterWidget->SetLayer(EWidgetLayer::Layer3, DamageWidgetInstance);

				DamageWidgetInstance->SetVisibility(ESlateVisibility::Visible);

				UPJHealthSet* HealthSet = Character->GetGameAttribute<UPJHealthSet>();

				if (HealthSet != nullptr)
				{
					int HP = (int)HealthSet->GetHealth();
					
					DamageWidgetInstance->PlayDamage(HP, ScreenPosition);	
				}
				
				SNPLUGIN_LOG(TEXT("Damage Effect is Enabled."));
			}
		}
	}
}
