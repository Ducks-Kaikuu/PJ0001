// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/NPC/PJEnemy.h"

#include "AbilitySystemComponent.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJ0000/Character/Abilities/Attributes/PJHealthSet.h"

APJEnemy::APJEnemy(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	HealthSet = CreateDefaultSubobject<UPJHealthSet>(TEXT("Health"));
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

void APJEnemy::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Damage"));
}
