// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SNDamageWithChooserComponent.generated.h"


class UChooserTable;
class UPlayMontageCallbackProxy;
struct FGameplayTagContainer;
struct FGameplayEffectContextHandle;

//!@{@defgroup キャラクター用コンポーネント
//!@{
//----------------------------------------------------------------------//
//
//! @brief ダメージ用コンポーネント
//! Chooserでダメージモーションの管理してます。
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNDamageWithChooserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNDamageWithChooserComponent();

	UPlayMontageCallbackProxy* PlayDamageAnimation(const FGameplayTagContainer& DamageTags, bool bAddToOwner=true);

	void SetDamagedEffectContextHandle(const FGameplayEffectContextHandle& DamagedHandle);

	void ResetDamagedEffectContextHandle();

	const FGameplayEffectContextHandle& GetDamagedEffectContextHandle() const;

	void SetDamageGuid(FGuid guid);

	FGuid GetDamageGuid() const;
	
	virtual void Death(){}
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	TObjectPtr<UChooserTable> DamageAnimationChooser = nullptr;

	FGameplayEffectContextHandle DamagedEffectContextHandle;

	FGuid DamageGuid;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	bool bDebugDraw = false;
#endif
	
};
//! @}
//! @}

FORCEINLINE void USNDamageWithChooserComponent::SetDamagedEffectContextHandle(const FGameplayEffectContextHandle& DamagedHandle)
{
	DamagedEffectContextHandle = DamagedHandle;
}

FORCEINLINE void USNDamageWithChooserComponent::ResetDamagedEffectContextHandle()
{
	DamagedEffectContextHandle.Clear();
}

FORCEINLINE const FGameplayEffectContextHandle& USNDamageWithChooserComponent::GetDamagedEffectContextHandle() const
{
	return DamagedEffectContextHandle;
}

FORCEINLINE void USNDamageWithChooserComponent::SetDamageGuid(FGuid guid)
{
	DamageGuid = guid;
}

FORCEINLINE FGuid USNDamageWithChooserComponent::GetDamageGuid() const
{
	return DamageGuid;
}