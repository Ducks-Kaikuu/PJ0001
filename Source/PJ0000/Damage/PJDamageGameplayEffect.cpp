// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Damage/PJDamageGameplayEffect.h"
#include "PJ0000/Damage/PJGameplayEffectComponent.h"

void UPJDamageGameplayEffect::PostInitProperties()
{
	Super::PostInitProperties();
#if 0
	if (DamageComponentClass != nullptr)
	{
		Component = NewObject<UGameplayEffectComponent>(this, DamageComponentClass);
	
		GEComponents.Add(Component);
	}
#endif
}

void UPJDamageGameplayEffect::PostLoad()
{
	Super::PostLoad();
#if 0
	if (DamageComponentClass != nullptr)
	{
		Component = NewObject<UGameplayEffectComponent>(this, DamageComponentClass);
	
		GEComponents.Add(Component);
	}
#endif
}
