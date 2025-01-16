// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "SNAttributeSet.generated.h"

class AActor;
class UWorld;
class USNAbilitySystemComponent;

struct FGameplayEffectSpec;

//!< 属性にアクセスして初期化するためのヘルパー関数のセットを定義。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class USNAbilitySystemComponent;
//----------------------------------------------------------------------//
//
//! @brief 属性イベントをブロードキャストするために使用されるデリゲート： 
//! @param EffectInstigator	加害者アクター
//! @param EffectCauser		被害者アクター
//! @param EffectSpec		変更内容
//! @param EffectMagnitude	クランプ前の値
//! @param OldValue			変更前の値
//! @param NewValue			変更後の値
//----------------------------------------------------------------------//
DECLARE_MULTICAST_DELEGATE_SixParams(FSNAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);


//----------------------------------------------------------------------//
//
//! @brief 属性管理クラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	USNAttributeSet();

	virtual UWorld* GetWorld() const override;

	USNAbilitySystemComponent* GetAbilitySystemComponent() const;
};
