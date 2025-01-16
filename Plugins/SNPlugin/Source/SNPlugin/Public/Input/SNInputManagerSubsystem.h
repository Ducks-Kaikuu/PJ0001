// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SNInputManagerSubsystem.generated.h"

class UInputMappingContext;

//!@{@defgroup インプット
//!@{
//----------------------------------------------------------------------//
//
//! @brief インプットマッピングコンテキストのサブシステム
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNInputManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	//! @{@name 初期化処理
	void Initialize(FSubsystemCollectionBase& Collection) override;
	//! @}
	
	//! @{@name インプットコンテキストを登録
	void AddInputContext(const FName& Name, UInputMappingContext* InputMappingContext);
	//! @}
	
	//! @{@name インプットコンテキストを削除
	void RemoveInputContext(const FName& Name);
	//! @}
	
	//! @{@name 全てのインプットマッピングコンテキストを削除
	void RemoveAllInputContext(APlayerController* PlayerController);
	//! @}
	
	//! @{@name インプットマッピングコンテキストを有効化
	bool EnableInputMapping(APlayerController* PlayerController, const FName& Name, bool bClearMapping=true);
	//! @}
	
	//! @{@name インプットマッピングコンテキストを無効化
	bool DisableInputMapping(APlayerController* PlayerController, const FName& Name);
	//! @}
	
	//! @{@name 指定された名前のインプットマッピングコンテキストが登録済みかチェック
	bool IsExist(const FName& Name) const ;
	//! @}
	
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UInputMappingContext>> InputMapContextMap;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief 指定された名前のインプットマッピングコンテキストが登録済みかチェック
//
//! @param Name 登録名
//
//! @retval true : 登録済み / false : 未登録
//
//----------------------------------------------------------------------//
FORCEINLINE bool USNInputManagerSubsystem::IsExist(const FName& Name) const {
	return (InputMapContextMap.Contains(Name));
}
