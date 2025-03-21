// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/PJCharacter.h"

#include "Abilities/Attributes/PJHealthSet.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APJCharacter::APJCharacter(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer){

}

bool APJCharacter::IsLanded() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		if((MovementComponent->MovementMode == MOVE_Flying)
		|| (MovementComponent->MovementMode == MOVE_Falling))
		{
			return false;
		}
		
	};
		
	return true;
}

void APJCharacter::UpdateCamera()
{
}

void APJCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent != nullptr)
	{
		const UPJHealthSet* Health = GetGameAttribute<UPJHealthSet>();

		if (Health != nullptr)
		{
			Health->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
		}
	}
}

void APJCharacter::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	
}
