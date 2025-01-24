// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/NPC/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"
#include "Scene/SNSceneBase.h"
#include "UI/Widget/PJDamageWidget.h"
#include "UI/Widget/SNMasterWidget.h"
#include "Utility/SNUtility.h"

APJEnemy::APJEnemy(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	HealthSet = CreateDefaultSubobject<UPJHealthSet>(TEXT("Health"));

	MovePositionComponent = CreateDefaultSubobject<USNMovePositionComponent>(TEXT("PositionComponent"));
}

void APJEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if 1
	const UPJHealthSet* Health = Cast<const UPJHealthSet>(AbilitySystemComponent->GetAttributeSet(UPJHealthSet::StaticClass()));

	if (Health != nullptr)
	{
		FString Str = FString(TEXT("Health : "))  +  FString::SanitizeFloat(Health->GetHealth());
		
		//UKismetSystemLibrary::PrintString(GetWorld(), *Str);
	}
#endif
}

void APJEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		const UPJHealthSet* Health = AbilitySystemComponent->GetSet<UPJHealthSet>();

		if (Health != nullptr)
		{
			Health->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
		}
	}
}

void APJEnemy::DrawDamage(int Damage)
{
	USNGameInstance* GameInstance = SNUtility::GetGameInstance<USNGameInstance>();
	
	UPJDamageWidget* DamageWidget = CreateWidget<UPJDamageWidget>(GameInstance, DamaageWidget, TEXT("Damage"));

	if (DamageWidget != nullptr)
	{
		ASNSceneBase* Scene(GameInstance->GetCurrentScene());

		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());

		if ((Scene != nullptr) && (PlayerController != nullptr))
		{
			USNMasterWidget* MasterWidget = Scene->GetMasterWidget();

			if (MasterWidget != nullptr)
			{
				FVector WorldPosition(GetActorLocation());

				//WorldPosition.Z += 150.0f;

				FVector2D ScreenPosition = FVector2D::ZeroVector;

				PlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition);

				ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(GetWorld());
				
				MasterWidget->SetLayer(EWidgetLayer::Layer3, DamageWidget);

				DamageWidget->SetVisibility(ESlateVisibility::Visible);

				int HP = (int)HealthSet->GetHealth();
				
				DamageWidget->PlayDamage(HP, ScreenPosition);

				

				SNPLUGIN_LOG(TEXT("Damage Effect is Enabled."));
			}
		}
	}
}

void APJEnemy::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"));
}
