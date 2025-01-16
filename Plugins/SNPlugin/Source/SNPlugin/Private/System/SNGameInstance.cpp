// Fill out your copyright notice in the Description page of Project Settings.
#include "System/SNGameInstance.h"

#include "OnlineSubsystemUtils.h"
#include "SNDef.h"
#include "..\..\Public\Online\SNOnlineSystem.h"
#include "GameplayTags/SNGameplayTags.h"
#include "Data/SNContentAssetManager.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Sound/SNSoundManager.h"
#include "Utility/SNUtility.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNGameInstance::USNGameInstance()
:Super()
,ContentAssetManagerClass(USNContentAssetManager::StaticClass())
{
	
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//----------------------------------------------------------------------//
void USNGameInstance::Init(){
	
	Super::Init();
	
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	
	if(ensure(ComponentManager)){
		ComponentManager->RegisterInitState(SNGameplayTags::InitState_Spawned        , false, FGameplayTag());
		ComponentManager->RegisterInitState(SNGameplayTags::InitState_DataAvailable  , false, SNGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(SNGameplayTags::InitState_DataInitialized, false, SNGameplayTags::InitState_DataAvailable);
	}
	// クラス情報をロード
	UClass* AssetClass = ContentAssetManagerClass.LoadSynchronous();
	// nullチェック
	if(AssetClass != nullptr){
		// インスタンスを生成
		ContentAssetManager = NewObject<USNContentAssetManager>(this, AssetClass);
	}
	
	SNPLUGIN_ASSERT(ContentAssetManager != nullptr, TEXT("ContentAssetManager Is Null"));
	
	ContentAssetManager->SetupDLCContents();
	// クラス情報が設定されているかチェック
	if(OnlineSystemClass.IsNull() == false){
		// クラス情報をローdお
		UClass* OnlineClass = OnlineSystemClass.LoadSynchronous();
		// ロードに成功したかチェック
		if(OnlineClass != nullptr){
			// オブジェクトを生成
			OnlineSystem = NewObject<USNOnlineSystem>(this, OnlineClass);
			
			if(OnlineSystem != nullptr){
				
				SNPLUGIN_LOG(TEXT("Online System is Enabled."));
				// オンライン処理を初期化
				OnlineSystem->Initialize();
			}
		}
	}
	// サウンド管理クラスを生成
	if(SoundManagerClass !=nullptr){
		
		SoundManager = NewObject<USNSoundManager>(this, SoundManagerClass.Get());
		
		if(SoundManager != nullptr){
			SoundManager->Initialize();
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief ゲームのインスタンスに使用するOnlineSessionクラスを取得
//
//! @retval ゲームのインスタンスに使用するOnlineSessionクラス
//
//----------------------------------------------------------------------//
TSubclassOf<UOnlineSession> USNGameInstance::GetOnlineSessionClass(){
	return OnlineSessionClass;
}

//----------------------------------------------------------------------//
//
//! @brief サウンド管理クラスを取得
//
//! @retval サウンド管理クラス
//
//----------------------------------------------------------------------//
USNSoundManager* USNGameInstance::GetSoundManager(){
	return SoundManager;
}

//----------------------------------------------------------------------//
//
//! @brief オンラインシステムのインスタンスを取得
//
//! @retval オンラインシステムのインスタンス
//
//----------------------------------------------------------------------//
USNOnlineSystem* GetOnlineSystem(){
	
	USNGameInstance* GameInstance(SNUtility::GetGameInstance<USNGameInstance>());
	
	USNOnlineSystem* OnlineSystem = nullptr;
	
	if(GameInstance != nullptr){
		OnlineSystem = GameInstance->GetOnlineSystem();
	}
	
	return OnlineSystem;
}

//----------------------------------------------------------------------//
//
//! @brief サウンド管理クラスのインスタンスを取得
//
//! @retval サウンド管理クラスのインスタンス
//
//----------------------------------------------------------------------//
USNSoundManager* GetSoundManater(){
	
	USNGameInstance* GameInstance(SNUtility::GetGameInstance<USNGameInstance>());
	
	USNSoundManager* SoundManager = nullptr;
	
	if(GameInstance != nullptr){
		SoundManager = GameInstance->GetSoundManager();
	}
	
	return SoundManager;
}


//----------------------------------------------------------------------//
//
//! @brief 常駐用システム音コンポーネントを取得
//
//! @retval 常駐用システム音コンポーネント
//
//----------------------------------------------------------------------//
USNAudioComponent* GetCoreSoundComponent(){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	USNAudioComponent* CoreSoundComponent = nullptr;
	
	if(SoundManager != nullptr){
		CoreSoundComponent = SoundManager->GetCoreSoundComponent();
	}
	
	return CoreSoundComponent;
}
