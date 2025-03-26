// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "SNDamageComponent.h"
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
class SNPLUGIN_API USNDamageWithChooserComponent : public USNDamageComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNDamageWithChooserComponent();

	UPlayMontageCallbackProxy* PlayDamageAnimation(const FGameplayTagContainer& DamageTags, bool bAddToOwner=true);

	DECLARE_MULTICAST_DELEGATE(FOnMontagePlayEnd);
	FOnMontagePlayEnd OnMontagePlayEndDelegate;

	DECLARE_MULTICAST_DELEGATE(FOnMontageInterrupted);
	FOnMontageInterrupted OnMontageInterruptedDelegate;

	DECLARE_MULTICAST_DELEGATE(FOnMontageBlendOut);
	FOnMontageBlendOut OnMontageBlendOutDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyBegin, FName);
	FOnNotifyBegin OnNotifyBeginDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyEnd, FName);
	FOnNotifyEnd OnNotifyEndDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void OnMontagePlayEnd(FName NotifyName);

	UFUNCTION()
	virtual void OnMontageInterrupted(FName NotifyName);

	UFUNCTION()
	virtual void OnMontageBlendOut(FName NotifyName);

	UFUNCTION()
	virtual void OnNotifyBegin(FName NotifyName);

	UFUNCTION()
	virtual void OnNotifyEnd(FName NotifyName);
	
private:
	
	UPROPERTY()
	TObjectPtr<UPlayMontageCallbackProxy> MontageProxy = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	TObjectPtr<UChooserTable> DamageAnimationChooser = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	bool bDebugDraw = false;
#endif
	
};
//! @}
//! @}
