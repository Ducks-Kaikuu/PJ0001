// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "PJDamageWithChooserComponent.generated.h"

class USNUserWidgetBase;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ0000_API UPJDamageWithChooserComponent : public USNDamageWithChooserComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Damage")
	void DrawDamage(int Damage);

	virtual void Death() override;

	void ResetLoopCount();
	
	void AddLoopCount(int Num);

	bool IsLoopEnd(int Num) const ;

	void ResetAirDamageTimer();

	void AddAirDamageTimer(float Time);

	void StartResumeTimer(float Time, const FName& SectionName);

	bool IsAirDamage(float Time) const ;

	void SetDamageAbilityTags(const FGameplayTagContainer& DamageAbilityTags);

protected:
	
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void OnResumeTimerDelegate();

	virtual void OnMontagePlayEnd(FName NotifyName) override;

	virtual void OnNotifyBegin(FName NotifyName) override;

	void DissolveStart();

	UFUNCTION()
	void DissoleExecute();

	UPROPERTY(EditAnywhere, Category="Death", meta = (ClampMin=0.016f, ClampMax=1000.0f))
	FGameplayTag DamageState;

	UPROPERTY()
	FGameplayTagContainer DamageTags;

	UPROPERTY(EditAnywhere, Category="Death", meta = (ClampMin=0.016f, ClampMax=1000.0f))
	float DeathTime = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Death")
	float DeathSpeed = 0.016f;

	float DeathCount = 0.0f;

	FTimerHandle DeathTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Damage")
	TSubclassOf<USNUserWidgetBase> DamageWidget = nullptr;

	int LoopCount = 0;

	float AirDamageTimer = 0.0f;

	FTimerHandle ResumeTimerHandle;

	float ResumeTime = 0.0f;

	FName NextSectionName = NAME_None;
};

FORCEINLINE void UPJDamageWithChooserComponent::ResetLoopCount()
{
	LoopCount = 0;
}

FORCEINLINE bool UPJDamageWithChooserComponent::IsLoopEnd(int Num) const
{
	return (LoopCount >= Num);
}

FORCEINLINE void UPJDamageWithChooserComponent::SetDamageAbilityTags(const FGameplayTagContainer& DamageAbilityTags)
{
	DamageTags = DamageAbilityTags;
}

FORCEINLINE void UPJDamageWithChooserComponent::ResetAirDamageTimer()
{
	AirDamageTimer = 0.0f;
}

FORCEINLINE bool UPJDamageWithChooserComponent::IsAirDamage(float Time) const
{
	return (AirDamageTimer >= Time);
}