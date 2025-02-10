// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/NPC/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Channels/MovieSceneTimeWarpChannel.h"
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
	//HealthSet = CreateDefaultSubobject<UPJHealthSet>(TEXT("Health"));

	MovePositionComponent = CreateDefaultSubobject<USNMovePositionComponent>(TEXT("PositionComponent"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void APJEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if 1
	const UPJHealthSet* Health = Cast<const UPJHealthSet>(AbilitySystemComponent->GetAttributeSet(UPJHealthSet::StaticClass()));

	if (Health != nullptr)
	{
		FString Str = FString(TEXT("Health : "))  +  FString::SanitizeFloat(Health->GetHealth());
		
		UKismetSystemLibrary::PrintString(GetWorld(), *Str);
	}
#endif

	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	if (AIController != nullptr)
	{
		//AIController->Restart();
	}
}

void APJEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APJEnemy::OnPerceptionUpdated);
	}
	
	
	const UPJHealthSet* Health = GetGameAttribute<UPJHealthSet>();

	if (Health != nullptr)
	{
		Health->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	}
}

void APJEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PerceptionComponent->OnPerceptionUpdated.RemoveDynamic(this, &APJEnemy::OnPerceptionUpdated);
}

void APJEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (auto& Actor: UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		
		PerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);
		
		if (PerceptionInfo.LastSensedStimuli.Num() > 0)
		{
			for (auto& Stimulus: PerceptionInfo.LastSensedStimuli)
			{
				if (Stimulus.WasSuccessfullySensed() == true)
				{
					UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Percept"));
				}
			}
		}
	}
}

UPlayMontageCallbackProxy* APJEnemy::PlayAnimMontageByActionTag()
{
	if (ChooserTable == nullptr)
	{
		return nullptr;
	}

	float PlayRate = 1.0f;
	float StartTime = 0.0f;
	
 	UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(this, ChooserTable, this, PlayRate, StartTime);

	if (AnimMontage != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			if (AnimInstance->Montage_IsPlaying(AnimMontage) == false)
			{
				MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), AnimMontage, 1.0f, 0.0f);
				
				SNPLUGIN_LOG(TEXT("Motion is %s"), *AnimMontage->GetName());
			}
		}
		
		return MontageProxy;
	}

	return nullptr;
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