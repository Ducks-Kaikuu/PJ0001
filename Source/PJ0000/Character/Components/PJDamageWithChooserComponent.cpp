// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/PJDamageWithChooserComponent.h"

#include "SNDef.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Abilities/Attributes/PJHealthSet.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNMaterialComponent.h"
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

	if (DamageWidget == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("PJDamageWithChooserComponent::DrawDamage - Damage Widget is nullptr."));

		return;
	}
	
	USNGameInstance* GameInstance = SNUtility::GetGameInstance<USNGameInstance>();
	
	UPJDamageWidget* DamageWidgetInstance = CreateWidget<UPJDamageWidget>(Character->GetWorld(), DamageWidget, TEXT("Damage"));

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

				const UPJHealthSet* HealthSet = Character->GetGameAttribute<UPJHealthSet>();

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

void UPJDamageWithChooserComponent::Death()
{
	Super::Death();

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("PJDamageWithChooserComponent::Death - Character is nullptr."));

		return;
	}

	const UPJHealthSet* HealthSet = Character->GetGameAttribute<UPJHealthSet>();

	if ((HealthSet == nullptr) || (HealthSet->GetHealth() > 0))
	{
		return;;
	}

	DissolveStart();
}

void UPJDamageWithChooserComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPJDamageWithChooserComponent::OnMontagePlayEnd(FName NotifyName)
{
	Super::OnMontagePlayEnd(NotifyName);
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		if (DamageState.IsValid())
		{
			Character->RemoveActionTag(DamageState);	
		}

		if (DamageTags.IsValid())
		{
			Character->RemoveActionTagContainer(DamageTags);;

			DamageTags.Reset();
		}
	}
}

void UPJDamageWithChooserComponent::OnNotifyBegin(FName NotifyName)
{
	Super::OnNotifyBegin(NotifyName);
}

void UPJDamageWithChooserComponent::DissolveStart()
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character == nullptr)
	{
		return;
	}

	DeathCount = 0.0f;

	Character->GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &UPJDamageWithChooserComponent::DissoleExecute, DeathSpeed, true);
}

void UPJDamageWithChooserComponent::DissoleExecute()
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));
	
	DeathCount += DeathSpeed;

	DeathCount = FMath::Clamp(DeathCount, 0.0f, DeathTime);

	USNMaterialComponent* MaterialComponent = Character->FindComponentByClass<USNMaterialComponent>();

	if (MaterialComponent != nullptr)
	{
		MaterialComponent->SetScalarParameterValue(TEXT("Werewolf"), TEXT("DissolveAlpha"), DeathCount/DeathTime);
	}

	if (DeathCount >= DeathTime)
	{
		Character->SetActorEnableCollision(false);

		Character->SetActorHiddenInGame(true);

		Character->GetWorldTimerManager().ClearTimer(DeathTimerHandle);

		Character->Destroy();
	}
}

void UPJDamageWithChooserComponent::AddLoopCount(int Num)
{
	LoopCount += Num;

	if (LoopCount < 0)
	{
		LoopCount = 0;
	}
}
