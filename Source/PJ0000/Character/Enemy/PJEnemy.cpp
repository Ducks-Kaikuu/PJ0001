// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Enemy/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Character/Components/SNMaterialComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"
#include "System/SNBlueprintFunctionLibrary.h"
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
		
		//UKismetSystemLibrary::PrintString(GetWorld(), *Str);
	}
#endif

	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	if (AIController != nullptr)
	{
		//AIController->Restart();
	}

	if (IsDead())
	{
		//Dissolve(10.0f);
	}
}

void APJEnemy::SetAttackMotionDelegate(UPlayMontageCallbackProxy* Proxy, const FGameplayTag& Tag)
{
	if (Proxy != nullptr)
	{
		if (Proxy->OnCompleted.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnCompleted.AddDynamic(this, &APJEnemy::OnAttackMotionEndplayMontage);
		}

		if (Proxy->OnInterrupted.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnInterrupted.AddDynamic(this, &APJEnemy::OnAttackMotionEndplayMontage);
		}
					
		if (Proxy->OnBlendOut.Contains(this, TEXT("OnAttackMotionEndplayMontage")) == false)
		{
			Proxy->OnBlendOut.AddDynamic(this, &APJEnemy::OnAttackMotionEndplayMontage);
		}
	}
	
	AttackTag = Tag;
}

void APJEnemy::OnAttackMotionEndplayMontage(FName NotifyName)
{
	if (AttackTag.IsValid() == true)
	{
		RemoveActionTag(AttackTag);
	}

	ResetMontagePlayProxy();
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

	USNMaterialComponent* MaterialComponent = FindComponentByClass<USNMaterialComponent>();

	if (MaterialComponent != nullptr)
	{
		MaterialComponent->CreateMaterialInstanceDynamic();
	}

	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	if (AIController != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(SNUtility::GetPlayerController<APlayerController>());

		if (PlayerController != nullptr)
		{
			AIController->SetPlayerKey(PlayerController->GetPawn());
		}
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

void APJEnemy::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (OnLanded.IsBound() == true)
	{
		OnLanded.Execute(Hit);
	}
}

void APJEnemy::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"));

	if (NewValue <= 0.0f)
	{
		SNPLUGIN_LOG(TEXT("HP is 0 : %s"), *DamageInstigator->GetActorLabel());
	}
}

float APJEnemy::GetMaxWaldSpeed() const
{
	UCharacterMovementComponent* Component = GetCharacterMovement();

	if (Component != nullptr)
	{
		return Component->MaxWalkSpeed;
	}

	return 0.0f;
}

