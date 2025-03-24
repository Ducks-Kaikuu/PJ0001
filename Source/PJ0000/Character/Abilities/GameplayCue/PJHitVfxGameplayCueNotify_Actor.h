// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "PJHitVfxGameplayCueNotify_Actor.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PJ0000_API APJHitVfxGameplayCueNotify_Actor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	
	APJHitVfxGameplayCueNotify_Actor();

	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

private:

	UPROPERTY(EditAnywhere, Category = "Vfx")
	UNiagaraSystem* HitVfx = nullptr;

	UPROPERTY(EditAnywhere, Category = "Vfx")
	UNiagaraSystem* EnemyHitVfx = nullptr;

	UPROPERTY(EditAnywhere, Category = "Vfx")
	UNiagaraSystem* PlayerHitVfx = nullptr;

	UPROPERTY(EditAnywhere, Category = "Vfx")
	FVector Scale = FVector::OneVector;

	
	
};
