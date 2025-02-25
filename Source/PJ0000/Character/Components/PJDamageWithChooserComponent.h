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

private:

	void DissolveStart();

	UFUNCTION()
	void DissoleExecute();
	
	UPROPERTY(EditAnywhere, Category="Death", meta = (ClampMin=0.016f, ClampMax=1000.0f))
	float DeathTime = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Death")
	float DeathSpeed = 0.016f;

	float DeathCount = 0.0f;

	FTimerHandle DeathTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Damage")
	TSubclassOf<USNUserWidgetBase> DamageWidget = nullptr;

	int LoopCount = 0;

};

FORCEINLINE void UPJDamageWithChooserComponent::ResetLoopCount()
{
	LoopCount = 0;
}

FORCEINLINE bool UPJDamageWithChooserComponent::IsLoopEnd(int Num) const
{
	return (LoopCount >= Num);
} 