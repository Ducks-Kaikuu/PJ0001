// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/GameplayCue/PJHitVfxGameplayCueNotify_Actor.h"

#include "SNDef.h"

APJHitVfxGameplayCueNotify_Actor::APJHitVfxGameplayCueNotify_Actor()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag((TEXT("GameplayCue.HitVfx")));
}

void APJHitVfxGameplayCueNotify_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	SNPLUGIN_LOG(TEXT("Hit Vfx Handler Called Event id = %d"), EventType);
}
