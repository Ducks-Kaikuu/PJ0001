// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/SNCharacterBase.h"
#include "UI/Widget/SNUserWidgetBase.h"
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
class PJ0000_API APJEnemy : public ASNCharacterBase
{
	GENERATED_BODY()
public:
	
	APJEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	void DrawDamage(int Damage);

	UPlayMontageCallbackProxy* PlayAnimMontageByActionTag();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	float GetVelocity2D() const ;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UPROPERTY()
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent = nullptr;;
	
	UPROPERTY()
	TObjectPtr<USNMovePositionComponent> MovePositionComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UPJHealthSet> HealthSet;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent = nullptr;;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChooserTable> ChooserTable = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USNUserWidgetBase> DamaageWidget = nullptr;

	UPROPERTY(EditAnywhere)
	float VisionAngle = 60.0f;

	UPROPERTY()
	TObjectPtr<UPlayMontageCallbackProxy> MontageProxy = nullptr;
};



