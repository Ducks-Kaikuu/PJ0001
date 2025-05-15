// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJEnemyBase.h"
#include "Character/Base/SNCharacterBase.h"
#include "PJEnemy.generated.h"

class UPlayMontageCallbackProxy;
class UChooserTable;
class UAIPerceptionComponent;
class UPawnSensingComponent;
class USNMovePositionComponent;
class USNUserWidgetBase;
struct FGameplayEffectSpec;

class UPJHealthSet;

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PJ0000_API APJEnemy : public APJEnemyBase
{
	GENERATED_BODY()
public:
	
	APJEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UPROPERTY()
	TObjectPtr<USNMovePositionComponent> MovePositionComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent = nullptr;;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<UMaterialInstanceDynamic>> MaterialInstanceDynamicInstArray;

	FTimerHandle DissolveTimerHandle;
};
