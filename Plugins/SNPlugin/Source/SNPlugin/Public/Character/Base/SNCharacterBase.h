// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Character/SNCharacterDef.h"
#include "Character/AbilitySystem/SNAttributeSet.h"

#include "SNCharacterBase.generated.h"

class USNAttributeSet;
class USNAbilitySystemComponent;
class USNLocomotionComponent;

//!@{@defgroup キャラクター制御
//!@{

// 上半身、下半身の制御をベースに入れるか悩みましたが…。基本機能なので入れました。
// 大したコストでもないし…。

//----------------------------------------------------------------------//
//
//! @brief キャラクター制御用ベースクラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASNCharacterBase(const FObjectInitializer& Initializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USNAbilitySystemComponent* GetAbilitySystemComponent() const ;
	
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//! @{@name ステートの設定
	void SetCurrentState(const FName& Name, ECharacterStateType Type=ECharacterStateType::Full);
	//! @}
	
	//! @{@name 現在のステートを取得
	UFUNCTION(BlueprintCallable, Category="SN|Character")
	FName GetCurrentState(ECharacterStateType Type=ECharacterStateType::Full) const ;
	//! @}
	
	//! @{@name 1つ前のステートを取得
	UFUNCTION(BlueprintCallable, Category="SN|Character")
	FName GetPrevState(ECharacterStateType Type=ECharacterStateType::Full) const ;
	//! @}
	
	//! @{@name 現在のステートかチェック
	UFUNCTION(BlueprintCallable, Category = "SN|Character", meta=(BlueprintThreadSafe))
	bool IsCurrentState(const FName& State, ECharacterStateType Type=ECharacterStateType::Full) const;
	//! @}
	
	//! @{@name 1つ前のステートかチェック
	UFUNCTION(BlueprintCallable, Category = "SN|Character", meta=(BlueprintThreadSafe))
	bool IsPreState(const FName& State, ECharacterStateType Type=ECharacterStateType::Full) const;
	//! @}
	
	//! @{@name アニメーションインスタンスを取得
	UAnimInstance* GetAnimInstance();
	//! @}
	
	//! @{@name ロコモーションコンポーネントを取得
	USNLocomotionComponent* GetLocomotionComponent();
	//! @}
	
	//! @{@name アニメーションシーケンスを再生
	UFUNCTION(BlueprintCallable, Category="SN|Animation")
	void PlaySequence(const FName& Name, const FName& Slot, float PlayRate=1.0f, float BlendIn = 0.05f, float BlendOut = 0.05f, float StartTime=0.0f, bool bLoop=false);
	//! @}
	
	//! @{@name アニメーションモンタージュを再生
	UFUNCTION(BlueprintCallable, Category="SN|Animation")
	void PlayMontage(const FName& Name, float PlayRate=1.0f, float StartTime=0.0f);
	//! @}

	void SetActinTag(const FGameplayTag& Tag);
	void SetActionTagContainer(const FGameplayTagContainer& TagContainer);

	void AddActionTagContainer(const FGameplayTagContainer& TagContainer);
	void AddActionTag(const FGameplayTag& Tag);

	void RemoveActionTag(const FGameplayTag& Tag);
	void RemoveActionTagContainer(const FGameplayTagContainer& TagContainer);
	void ResetAllActionTags();

	bool HasActionTag(const FGameplayTag& Tag) const;
	bool HasAnyActionTags(const FGameplayTagContainer& TagContainer) const ;
	bool HasAllActionTags(const FGameplayTagContainer& TagContainer) const ;

	UFUNCTION(BlueprintCallable, Category="SN|Action", meta=(BlueprintThreadSafe))
	FGameplayTagContainer GetActionTags() const;
		
	//! @{@name モンタージュのセクションへのジャンプ
	UFUNCTION(BlueprintCallable, Category="SN|Animation")
	void JumpMontageSection(const FName& Name, const FName& Section);
	//! @}
	
	//! @{@name ブレンドスペースのパラメータを設定
	UFUNCTION(BlueprintCallable, Category="SN|Animation")
	void SetBlendspaceParam(const FName& Key, const FVector& param);
	//! @}

	template<class T>
	T* GetGameAttribute();

	template<class T>
	const T* GetGameAttribute() const ;
	
	//!@ Satoshi Nishimura 2025/01/31 アクセッサを用意してもうまく動作せず、変数をBlueprintに出してやるとうまくいく…意味不明なChooser
	UPROPERTY(BlueprintReadOnly, Replicated)
	FGameplayTagContainer ActionTags;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//!< 上半身の現在のステート
	UPROPERTY()
	FName UppderBodyCurrentState = NAME_None;
	
	//!< 上半身の1つ前のステート
	UPROPERTY()
	FName UppderBodyPreStateName = NAME_None;
	
	//!< 下半身の現在のステート
	UPROPERTY()
	FName LowerBodyCurrentState = NAME_None;
	
	//!< 下半身の1つ前のステート
	UPROPERTY()
	FName LowerBodyPreStateName = NAME_None;
	
	//!< 全身の現在のステート
	UPROPERTY()
	FName FullBodyCurrentState = NAME_None;
	
	//!< 全身の1つ前のステート
	UPROPERTY()
	FName FullBodyPreStateName = NAME_None;

	UPROPERTY(EditAnywhere, Category="SN|Ability")
	TObjectPtr<USNAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
private:

	UPROPERTY(EditAnywhere, Category="SN|Ability")
	TArray<TSubclassOf<USNAttributeSet>> AttributeSetClass;

	UPROPERTY()
	TArray<TObjectPtr<USNAttributeSet>> AttributeSets;
	
	//! @{@name ステートの変更
	UFUNCTION(Server, Reliable, WithValidation)
	void SetCurrentState_OnServer(const FName& Name, ECharacterStateType Type);
	
	UFUNCTION(NetMulticast,Reliable, WithValidation)
	void SetCurrentState_OnMulticast(const FName& Name, ECharacterStateType Type);
	
	void InternalSetCurrentState(const FName& Name, ECharacterStateType Type);
	//! @}
	
	//! @{@name ブレンドスペースのパラメータを設定
	UFUNCTION(Server, Reliable)
	void SetBlendspaceParam_OnServer(const FName& Key, const FVector& Param);
	
	UFUNCTION(NetMulticast, Reliable)
	void SetBlendspaceParam_OnMulticast(const FName& Key, const FVector& Param);
	
	void IntervalBlendspaceParam(const FName& Key, const FVector& Param);
	//! @}
	
	//! @{@name アニメーションシーケンスを再生
	UFUNCTION(Server, Reliable)
	void PlaySequence_OnServer(const FName& Name, const FName& Slot, float PlayRate, float BlendIn, float BlendOut, float StartTime, bool bLoop);
	
	UFUNCTION(NetMulticast, Reliable)
	void PlaySequence_OnMulticast(const FName& Name, const FName& Slot, float PlayRate, float BlendIn, float BlendOut, float StartTime, bool bLoop);
	
	void InternalPlaySequence(const FName& Name, const FName& Slot, float PlayRate, float BlendIn, float BlendOut, float StartTime, bool bLoop);
	//! @}
	
	//! @{@name アニメーションモンタージュを再生
	UFUNCTION(Server, Reliable)
	void PlayMontage_OnServer(const FName& Name, float PlayRate, float StartTime);
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayMontage_OnMulticast(const FName& Name, float PlayRate, float StartTime);
	
	void InternalPlayMontage(const FName& Name, float PlayRate, float StartTime);
	//! @}
	
	//! @{@name モンタージュのセクションへのジャンプ
	UFUNCTION(Server, Reliable)
	void JumpMontageSection_OnServer(const FName& Name, const FName& Section);
	
	UFUNCTION(NetMulticast, Reliable)
	void JumpMontageSection_OnMulticast(const FName& Name, const FName& Section);
	
	void InternalJumpMontageSection(const FName& Name, const FName& Section);
	//! @}
};
//! @}
//! @}

FORCEINLINE USNAbilitySystemComponent* ASNCharacterBase::GetAbilitySystemComponent() const 
{
	return AbilitySystemComponent;
}

FORCEINLINE bool ASNCharacterBase::HasActionTag(const FGameplayTag& Tag) const
{
	return ActionTags.HasTag(Tag);
}
FORCEINLINE bool ASNCharacterBase::HasAnyActionTags(const FGameplayTagContainer& TagContainer) const
{
	return ActionTags.HasAny(TagContainer);
}

FORCEINLINE bool ASNCharacterBase::HasAllActionTags(const FGameplayTagContainer& TagContainer) const
{
	return ActionTags.HasAll(TagContainer);
}

template<class T>
FORCEINLINE T* ASNCharacterBase::GetGameAttribute()
{
	for (auto& GameAttribute : AttributeSets)
	{
		if ((GameAttribute != nullptr) && (GameAttribute->GetClass() == T::StaticClass())){
			return Cast<T>(GameAttribute);
		}
	}

	return nullptr;
}

template<class T>
FORCEINLINE const T* ASNCharacterBase::GetGameAttribute() const
{
	for (auto& GameAttribute : AttributeSets)
	{
		if ((GameAttribute != nullptr) && (GameAttribute->GetClass() == T::StaticClass())){
			return Cast<T>(GameAttribute);
		}
	}

	return nullptr;
}