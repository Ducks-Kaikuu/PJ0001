// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNAbilitySystemComponent.h"

#include "SNDef.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Utility/SNUtility.h"
#include "Character/AbilitySystem/SNAttributeSet.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//! @param ObjectInitializer イニシャライザ
//
//----------------------------------------------------------------------//
USNAbilitySystemComponent::USNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer){
	
	PrimaryComponentTick.bCanEverTick = true;
	
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

//----------------------------------------------------------------------//
//
//! @brief コンポーネントが処理を開始する前にコールされます。
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::BeginPlay(){
	
	Super::BeginPlay();
	
	SetComponentTickEnabled(false);
	
	TArray<FSoftObjectPath> assetList;
	
	for(auto& Path:AbilityClassList){
		assetList.Add(Path.ToSoftObjectPath());
	}
	// ロードをリクエスト
	StreamHandle = SNUtility::RequestAsyncLoad<USNAbilitySystemComponent>(assetList, this, &USNAbilitySystemComponent::LoadedAbility);
}

//----------------------------------------------------------------------//
//
//! @brief BPで設定されているアビリティのロード終了処理
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::LoadedAbility(){
	
	if(StreamHandle.IsValid()){
		
		TArray<UObject*> Assets;
		
		StreamHandle->GetLoadedAssets(Assets);
		
		for(auto& Asset : Assets){
			
			UClass* Class = Cast<UClass>(Asset);
			
			if(Class != nullptr){
				AddAbility(Class);
			}
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief GameplayTagCountContainerへゲームプレイタグコンテナを追加
//
//! @param Tags 追加するゲームプレイタグコンテナ
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::AddGameplayTagsCountContainer(const FGameplayTagContainer& Tags){
	AddLooseGameplayTags(Tags);
}

//----------------------------------------------------------------------//
//
//! @brief GameplayTagCountContainerからゲームプレイタグコンテナを削除
//
//! @param Tags 削除するゲームプレイタグコンテナ
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::RemoveGameplayTagsCountContainer(const FGameplayTagContainer& Tags){
	RemoveLooseGameplayTags(Tags);
}

//----------------------------------------------------------------------//
//
//! @brief GameplayTagCountContainerから全てのゲームプレイタグを削除
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::RemoveAllGameplayTagCountContainer(){
	GameplayTagCountContainer.Reset();
}

//----------------------------------------------------------------------//
//
//! @brief AbilitySystemComponentへアビリティを追加
//
//! @param AbilitClass アビリティのクラス情報
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::AddAbility(TSubclassOf<UGameplayAbility> AbilitClass){
	
	if(AbilitClass != nullptr){
		
		FGameplayAbilitySpec Spec(AbilitClass->GetDefaultObject<UGameplayAbility>());
		
		if(Spec.Handle.IsValid()){
			
			FGameplayAbilitySpecHandle Handle = GiveAbility(Spec);
			
			if(Handle.IsValid()){
				AbilityTaskList.Add(AbilitClass, Handle);
			} else {
				SNPLUGIN_ERROR(TEXT("[USNAbilitySystemComponent::AddAbility] : Failed to GiveAbility."));
			}
		} else {
			SNPLUGIN_ERROR(TEXT("[USNAbilitySystemComponent::AddAbility] : Ability Class is Invalidate."));
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief AbilitySystemComponentからアビリティを削除
//
//! @param AbilityClass アビリティのクラス情報
//
//----------------------------------------------------------------------//
void USNAbilitySystemComponent::RemoveAbility(TSubclassOf<UGameplayAbility> AbilityClass){
	
	if(AbilityTaskList.Contains(AbilityClass) == true){
		
		FGameplayAbilitySpecHandle Handle(AbilityTaskList[AbilityClass]);
		
		SetRemoveAbilityOnEnd(Handle);
		
		AbilityTaskList.Remove(AbilityClass);
	} 
}
