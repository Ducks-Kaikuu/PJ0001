// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SNAbilitySystemComponent.generated.h"

class USNAttributeSet;
struct FStreamableHandle;
class USNAttribute;

//!@{@defgroup キャラクター用コンポーネント
//!@{
//----------------------------------------------------------------------//
//
//! @brief アビリティシステムコンポーネント
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	
	//! @{@name デフォルトコンストラクタ
	USNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
	//! @}
	
	//! @{@name GameplayTagCountContainerへゲームプレイタグコンテナを追加
	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void AddGameplayTagsCountContainer(const FGameplayTagContainer& Tags);
	//! @}
	
	//! @{@name GameplayTagCountContainerからゲームプレイタグコンテナを削除
	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveGameplayTagsCountContainer(const FGameplayTagContainer& Tags);
	//! @}
	
	//! @{@name GameplayTagCountContainerから全てのゲームプレイタグを削除
	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveAllGameplayTagCountContainer();
	//! @}
	
	//! @{@name AbilitySystemComponentへアビリティを追加
	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void AddAbility(TSubclassOf<UGameplayAbility> AbilitClass);
	//! @}
	
	//! @{@name AbilitySystemComponentからアビリティを削除
	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveAbility(TSubclassOf<UGameplayAbility> AbilityClass);
	//! @}
	
protected:
	
	//! @{@name コンポーネントが処理を開始する前にコールされます。
	virtual void BeginPlay() override;
	//! @}
	
private:
	
	//! @{@name BPで設定されているアビリティのロード終了処理
	UFUNCTION()
	void LoadedAbility();
	//! @}
	
	//! アビリティクラスの非同期ロード用ハンドル
	TSharedPtr<FStreamableHandle> StreamHandle;
	
	//! 親のキャラクターに持たせるアビリティ
	UPROPERTY(EditAnywhere, Category = "SN|AbilitySystem")
	TArray<TSoftClassPtr<UGameplayAbility>> AbilityClassList;
	
	//! アビリティのクラス情報とハンドルの管理用マップ
	//!※ネイティブC++の場合はポインタをキーになんてしないように！
	UPROPERTY()
	TMap<TObjectPtr<UClass>, FGameplayAbilitySpecHandle> AbilityTaskList;
};
//! @}
//! @}
