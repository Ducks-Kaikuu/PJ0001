// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Engine/AssetManager.h"
#include "InputTriggers.h"
#include "SNInputConfig.generated.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
class UInputAction;
class USNActionBase;
class UEnhancedInputComponent;
class UInputMappingContext;

//!@{@defgroup インプット
//!@{
//----------------------------------------------------------------------//
//
//! @brief 入力アクションの設定
//
//----------------------------------------------------------------------//
USTRUCT(BlueprintType)
struct FSNInputAction{
	
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ExclusiveTags;
	
	//!< アクションへのポインタ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;
	
	//!< ゲームプレイタグで実行するアクション
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "Action"))
	TSoftClassPtr<USNActionBase> ActionClass = nullptr;
};

//----------------------------------------------------------------------//
//
//! @brief 入力制御用のコンフィグ
//
//! @note SNPlayerControllerに登録して使用。
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	//! @{@name デフォルトコンストラクタ
	USNInputConfig(const FObjectInitializer& ObjectInitializer);
	//! @}
	
	//! @{@name 入力周りの初期化処理
	bool	InitializeInput(FName Name, UObject* OwnerObject);
	//! @}
	
	//! @{@name コンフィグ名を取得
	const FName& GetConfigName() const ;
	//! @}
	
	//! @{@name インプットマッピングコンテキストを取得
	UInputMappingContext* GetInputMappingContext();
	//! @}

	//! @{@name 解放処理
	void Release();
	//! @}

private:
	
	//!< コンフィグの名前
	UPROPERTY()
	FName ConfigName=NAME_None;
	
	//!< マッピングコンテキスト
	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
	
	//!< 入力アクション用リスト
	UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = "InputAction"), Category="Input")
	TArray<FSNInputAction> InputActionList;
	
	//!< オーナー
	UPROPERTY()
	TObjectPtr<UObject> OwnerActor = nullptr;
	
	//!< インプットマッピングコンテキスト
	UPROPERTY()
	UInputMappingContext* IMCInstance = nullptr;
	
	//!< GCで消されないために保持
	UPROPERTY()
	TArray<TObjectPtr<USNActionBase>> ActionList;
	
	//!< 非同期ロード用のハンドル
	TSharedPtr<FStreamableHandle> StreamableHandle = nullptr;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief コンフィグ名を取得
//
//! @retval コンフィグ名
//
//----------------------------------------------------------------------//
FORCEINLINE const FName& USNInputConfig::GetConfigName() const {
	return ConfigName;
} 

//----------------------------------------------------------------------//
//
//! @brief インプットマッピングコンテキストを取得
//
//! @retval インプットマッピングコンテキスト
//
//----------------------------------------------------------------------//
FORCEINLINE UInputMappingContext* USNInputConfig::GetInputMappingContext(){
	return IMCInstance;
}
