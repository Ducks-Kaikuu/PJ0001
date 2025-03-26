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
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"
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

//	if (WidgetInstance != nullptr)
//	{
//		WidgetInstance = nullptr;
//	}
	
	WidgetInstance = CreateWidget<UPJDamageWidget>(Character->GetWorld(), DamageWidget, TEXT("Damage"));

	if (WidgetInstance != nullptr)
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
				
				MasterWidget->SetLayer(EWidgetLayer::Layer3, WidgetInstance.Get());

				WidgetInstance->SetVisibility(ESlateVisibility::Visible);

				const UPJHealthSet* HealthSet = Character->GetGameAttribute<UPJHealthSet>();

				if (HealthSet != nullptr)
				{
					int HP = (int)HealthSet->GetHealth();
					
					WidgetInstance->PlayDamage(HP, ScreenPosition);	
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

void UPJDamageWithChooserComponent::StartStrike()
{
	
}

void UPJDamageWithChooserComponent::BeginPlay()
{
	Super::BeginPlay();

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		Character->LandedDelegate.AddDynamic(this, &UPJDamageWithChooserComponent::OnCharacterLanded);
	}
}

void UPJDamageWithChooserComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		Character->LandedDelegate.RemoveDynamic(this, &UPJDamageWithChooserComponent::OnCharacterLanded);
	}
}

void UPJDamageWithChooserComponent::OnCharacterLanded(const FHitResult& Hit)
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		UAnimInstance* AnimInstance(Character->GetAnimInstance());

		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Resume(nullptr);;
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

void UPJDamageWithChooserComponent::AddAirDamageTimer(float Time)
{
	AirDamageTimer += Time;

	if (AirDamageTimer < 0)
	{
		AirDamageTimer = 0;
	}
}

void UPJDamageWithChooserComponent::StartResumeTimer(float Time, const FName& SectionName)
{
	NextSectionName = SectionName;

	ResumeTime = Time;

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		Character->GetWorldTimerManager().SetTimer(ResumeTimerHandle, this, &UPJDamageWithChooserComponent::OnResumeTimerDelegate, 1.0f/60.0f, true);
	}
}

void UPJDamageWithChooserComponent::OnResumeTimerDelegate()
{
	AddAirDamageTimer(1.0f/60.0f);

	if (IsAirDamage(ResumeTime))
	{
		ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

		if (Character != nullptr)
		{
			UAnimInstance* AnimInstance = Character->GetAnimInstance();

			if (AnimInstance != nullptr)
			{
				if (NextSectionName != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(NextSectionName);

					NextSectionName = NAME_None;
				}

				AnimInstance->Montage_Resume(nullptr);

				ResetAirDamageTimer();

			}

			Character->GetWorldTimerManager().ClearTimer(ResumeTimerHandle);
		}
	}
}

void UPJDamageWithChooserComponent::OnMontagePlayEnd(FName NotifyName)
{
	Super::OnMontagePlayEnd(NotifyName);
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		FGameplayTag StrikeDamage = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.Strike"));

		if (DamageTags.HasAny(FGameplayTagContainer(StrikeDamage)) == true)
		{
			return;
		}
		
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

void UPJDamageWithChooserComponent::OnMontageInterrupted(FName NotifyName)
{
	Super::OnMontageInterrupted(NotifyName);
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		FGameplayTag StrikeDamage = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.Strike"));

		if (DamageTags.HasAny(FGameplayTagContainer(StrikeDamage)) == true)
		{
			return;
		}
		
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

void UPJDamageWithChooserComponent::OnMontageBlendOut(FName NotifyName)
{
	Super::OnMontageBlendOut(NotifyName);

	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		FGameplayTag StrikeDamage = FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.Strike"));

		if (DamageTags.HasAny(FGameplayTagContainer(StrikeDamage)) == true)
		{
			return;
		}
		
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
