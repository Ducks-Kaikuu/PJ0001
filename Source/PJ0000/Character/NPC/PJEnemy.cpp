// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/NPC/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Base/SNPlayerBase.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"
#include "Scene/SNSceneBase.h"
#include "System/SNBlueprintFunctionLibrary.h"
#include "UI/Widget/PJDamageWidget.h"
#include "UI/Widget/SNMasterWidget.h"
#include "Utility/SNUtility.h"

APJEnemy::APJEnemy(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	HealthSet = CreateDefaultSubobject<UPJHealthSet>(TEXT("Health"));

	MovePositionComponent = CreateDefaultSubobject<USNMovePositionComponent>(TEXT("PositionComponent"));

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
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

	if (PawnSensingComponent != nullptr)
	{
		PawnSensingComponent->SetPeripheralVisionAngle(VisionAngle);

		if (PawnSensingComponent->OnSeePawn.Contains(this, TEXT("OnSeePlayer")) == false)
		{
			PawnSensingComponent->OnSeePawn.AddDynamic(this, &APJEnemy::OnSeePlayer);
		}
	}
	
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

void APJEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (PawnSensingComponent != nullptr)
	{
		PawnSensingComponent->OnSeePawn.RemoveDynamic(this, &APJEnemy::OnSeePlayer);
	}
}

void APJEnemy::OnSeePlayer(APawn* Pawn)
{
	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	ASNPlayerBase* Player = Cast<ASNPlayerBase>(Pawn);

	if ((AIController != nullptr) && (Player != nullptr))
	{
		float Dist = FVector::Distance(GetActorLocation(), Player->GetActorLocation());

		if (Dist > 50.f)
		{
			AIController->Restart();
		}
		
		AIController->SetPlayerKey(Player);

		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("See"));
	}

	if (ChooserTable != nullptr)
	{
		float PlayRate = 1.0f;
		float StartTime = 0.0f;
		
		UAnimMontage* AnimMontage(USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(this, ChooserTable, this, PlayRate, StartTime));

		if (AnimMontage != nullptr)
		{
			UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), const_cast<UAnimMontage*>(AnimMontage), 1.0f, 0.0f, NAME_None);
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

float APJEnemy::GetVelocity2D() const
{
	FVector Velocity = GetCharacterMovement()->Velocity;

	float Speed = Velocity.Size2D();

	if (GetCharacterMovement()->MovementMode != MOVE_Walking)
	{
		Speed = 0.0f;
	}

	return Speed;
}