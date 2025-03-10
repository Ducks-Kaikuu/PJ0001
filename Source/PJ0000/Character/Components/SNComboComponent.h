// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "SNComboComponent.generated.h"

class UPlayMontageCallbackProxy;
class UChooserTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PJ0000_API USNComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNComboComponent();

	void Start();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPlayMontageCallbackProxy* PerformMotionMatching(ACharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	float GetComboScore() const { return ComboScore; }

	void SetComboScore(float NewComboScore) { ComboScore = NewComboScore; }
	
	void SetAdvancedInput(bool Flag);

	bool IsAdvancedInput() const { return bAdvancedInput; }

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsLanded() const ;

protected:
	virtual void BeginPlay() override;
	
private:

	void FinishCombo();
	
	UFUNCTION()
	void OnMontagePlayEnd(FName NotiryName);
	
	UPROPERTY(EditDefaultsOnly, Category = "SN|Attack")
	TObjectPtr<UChooserTable> AttackAnimationChooser = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "SN|Attack")
	FGameplayTag AdvancedInputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, Category = "SN|Attack")
	FGameplayTag BranchTag = FGameplayTag();

	UPROPERTY()
	UPlayMontageCallbackProxy* MontageProxyInstance = nullptr;
	
	UPROPERTY()
	bool bAdvancedInput = false;

	UPROPERTY()
	float ComboScore = -1.0f;
};

FORCEINLINE void USNComboComponent::SetAdvancedInput(bool Flag)
{
	bAdvancedInput = Flag;
}