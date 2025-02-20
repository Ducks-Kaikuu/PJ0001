// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/GameplayCue/PJHitVfxGameplayCueNotify_Actor.h"

#include "SNDef.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/Components/PJDamageWithChooserComponent.h"

APJHitVfxGameplayCueNotify_Actor::APJHitVfxGameplayCueNotify_Actor()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag((TEXT("GameplayCue.HitVfx")));
}

void APJHitVfxGameplayCueNotify_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	UPJDamageWithChooserComponent* DamageComponent(MyTarget->FindComponentByClass<UPJDamageWithChooserComponent>());

	if (DamageComponent != nullptr)
	{
		FGameplayEffectContextHandle ContextHandle(DamageComponent->GetDamagedEffectContextHandle());

		if (ContextHandle.IsValid())
		{
			const FHitResult* HitResult(ContextHandle.GetHitResult());

			if (HitResult != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVfx, HitResult->ImpactPoint, FRotator::ZeroRotator, Scale);
			}
		}
	}
	
	
	SNPLUGIN_LOG(TEXT("Hit Vfx Handler Called Event id = %d"), EventType);
}
