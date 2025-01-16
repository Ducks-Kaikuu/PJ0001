// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "SNPlayablePawnInterface.generated.h"

enum class EInputActionValueType : uint8;
class USNInputConfig;
class  USNActionBase;
struct FInputActionValue;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USNPlayablePawnInterface : public UInterface
{
	GENERATED_BODY()
};

//----------------------------------------------------------------------//
//
//! @brief プレイ可能なポーン用のインターフェイス
//
//----------------------------------------------------------------------//
class SNPLUGIN_API ISNPlayablePawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	//! @{@name デフォルトコンストラクタ
	ISNPlayablePawnInterface();
	//! @}
	
	//! @{@name 入力された際に呼ばれるアクションを追加
	virtual void AddInputAction(const FGameplayTag& Tag, USNActionBase* Action) = 0;
	//! @}
	
	//! @{@name アクションを取得
	virtual USNActionBase* GetAction(const FGameplayTag& Tag) = 0;
	//! @}

	
	
	virtual int GetActionNum() const = 0;
	
	//! @{@name サーバー側でアクションを実行
	void ExecuteActionOnServer(const FGameplayTag& Name, const FInputActionValue& InputActionValue);
	//! @}
	
	void ExecuteActionOnMulticast(const FGameplayTag& Name, const FInputActionValue& InputActionValue);
	
protected:
	
	//! @{@name 入力からアクション実行用のレプリケーション
	UFUNCTION(Server, Reliable)
	virtual void ExecuteInputAction_OnServer(const FGameplayTag& Name, const FVector& InputValue, EInputActionValueType Type);
	//! @}
	
	//! @{@name 入力からアクション実行用のレプリケーション(クライアント)
	UFUNCTION(NetMulticast, Reliable)
	virtual void ExecuteInputAction_OnMulticast(const FGameplayTag& Name, const FVector& InputValue, EInputActionValueType Type);
	//! @}
	
	//! @{@name レプリケーションからアクション実行
	void ExexuteInputActionImplement(const FGameplayTag& Name, const FVector& InputValue, EInputActionValueType Type);
	//! @}
	
	//! @{@name アクションを実行
	void ExecuteAction(const FGameplayTag& Name, const FInputActionValue& InputActionValue);
	//! @}
};
