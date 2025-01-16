// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AbilitySystem/SNAttributeSet.h"
#include "SNDef.h"
#include "Character/Components/SNAbilitySystemComponent.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNAttributeSet::USNAttributeSet(){
	
}

//----------------------------------------------------------------------//
//
//! @brief 親クラスのワールドクラスを取得
//
//! @retval ワールドクラス
//
//----------------------------------------------------------------------//
UWorld* USNAttributeSet::GetWorld() const{
	// 親クラスを取得
	const UObject* Outer = GetOuter();
	
	SNPLUGIN_ASSERT(Outer != nullptr, TEXT("USNAttributeSet::GetWorld - Outer is nullptr."));
	
	return Outer->GetWorld();
}

//----------------------------------------------------------------------//
//
//! @brief AbilitySystemComponentを取得
//
//! @retval AbilitySystemComponent
//
//----------------------------------------------------------------------//
USNAbilitySystemComponent* USNAttributeSet::GetAbilitySystemComponent() const{
	return Cast<USNAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
