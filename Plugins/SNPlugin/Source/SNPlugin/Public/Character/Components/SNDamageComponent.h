// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "SNDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNDamageComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class USNAttackComponent;
	
public:	
	// Sets default values for this component's properties
	USNDamageComponent();

	FGuid GetDamageGuid() const;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Death(){}

	void ResetDamagedEffectContextHandle();

	const FGameplayEffectContextHandle& GetDamagedEffectContextHandle() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FGameplayEffectContextHandle DamagedEffectContextHandle;

private:

	void SetDamagedEffectContextHandle(const FGameplayEffectContextHandle& DamagedHandle);
	
	void SetDamageGuid(FGuid guid);

	FGuid DamageGuid;
};


FORCEINLINE void USNDamageComponent::SetDamagedEffectContextHandle(const FGameplayEffectContextHandle& DamagedHandle)
{
	DamagedEffectContextHandle = DamagedHandle;
}

FORCEINLINE void USNDamageComponent::ResetDamagedEffectContextHandle()
{
	DamagedEffectContextHandle.Clear();
}

FORCEINLINE const FGameplayEffectContextHandle& USNDamageComponent::GetDamagedEffectContextHandle() const
{
	return DamagedEffectContextHandle;
}

FORCEINLINE void USNDamageComponent::SetDamageGuid(FGuid guid)
{
	DamageGuid = guid;
}

FORCEINLINE FGuid USNDamageComponent::GetDamageGuid() const
{
	return DamageGuid;
}