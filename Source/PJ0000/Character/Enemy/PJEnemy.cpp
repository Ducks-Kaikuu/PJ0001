// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Enemy/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
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
}

void APJEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PerceptionComponent != nullptr)
	{
		PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APJEnemy::OnPerceptionUpdated);
	}
	
	const UPJHealthSet* Health(GetGameAttribute<UPJHealthSet>());
	
	if(Health != nullptr){
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

void APJEnemy::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"));

	if (NewValue <= 0.0f)
	{
		UPJDamageWithChooserComponent* DamageComponent(Cast<UPJDamageWithChooserComponent>(DamageCauser->FindComponentByClass<UPJDamageWithChooserComponent>()));

		if (DamageComponent != nullptr)
		{
			DamageComponent->Death();
		}
	}
}
