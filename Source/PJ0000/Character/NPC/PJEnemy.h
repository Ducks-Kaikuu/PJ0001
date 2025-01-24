// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/SNCharacterBase.h"
#include "UI/Widget/SNUserWidgetBase.h"
#include "PJEnemy.generated.h"

class USNMovePositionComponent;
class USNUserWidgetBase;
struct FGameplayEffectSpec;

class UPJHealthSet;

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PJ0000_API APJEnemy : public ASNCharacterBase
{
	GENERATED_BODY()
public:
	APJEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	void DrawDamage(int Damage);
	
protected:
	virtual void BeginPlay() override;
	
private:
	
	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UPROPERTY()
	TObjectPtr<USNMovePositionComponent> MovePositionComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPJHealthSet> HealthSet;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USNUserWidgetBase> DamaageWidget = nullptr;
};
