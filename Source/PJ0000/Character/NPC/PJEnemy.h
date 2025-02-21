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
	
	UPlayMontageCallbackProxy* PlayAnimMontageByActionTag();

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsDead() const;

	void Dissolve(float Time);

	DECLARE_DELEGATE_OneParam(FLandedDelegate, const FHitResult&);
	FLandedDelegate OnLanded;

	UFUNCTION()
	void DissolveDelegate();
	
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
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChooserTable> ChooserTable = nullptr;

	UPROPERTY()
	TObjectPtr<UPlayMontageCallbackProxy> MontageProxy = nullptr;

	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UMaterialInstance>> MaterialInstArray;

	UPROPERTY()
	TMap<FName, TObjectPtr<UMaterialInstanceDynamic>> MaterialInstanceDynamicInstArray;

	FTimerHandle DissolveTimerHandle;
};



