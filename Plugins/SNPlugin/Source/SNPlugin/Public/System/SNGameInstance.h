// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Online/SNOnlineSession.h"
#include "Sound/SNSoundManager.h"
#include "SNGameInstance.generated.h"

class USNSoundManager;
class ASNSceneBase;
class USNContentAssetManager;
class USNOnlineSystem;

//!@{@defgroup シングルトン
//!@{
//----------------------------------------------------------------------//
//
//! @brief ゲームインスタンスベースクラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	//! @{@name コンストラクタ
	USNGameInstance();
	//! @}
	
	//! @{@name 初期化処理
	virtual void Init() override ;
	//! @}
	
	//! @{@name ゲームのインスタンスに使用するOnlineSessionクラスを取得
	virtual TSubclassOf<UOnlineSession> GetOnlineSessionClass() override;
	//! @}
	
	//! @{@name データアセット管理クラスのポインタを取得
	USNContentAssetManager* GetContentAssetManager();
	//! @}
	
	//! @{@name オンラインシステムが有効かのチェック
	bool IsEnabledOnlineSystem() const ;
	//! @}
	
	//! @{@name オンラインシステムのインスタンスを取得
	USNOnlineSystem* GetOnlineSystem();
	//! @}
	
	//! @{@name 現在のシーンクラスを設定
	void SetCurrentScene(ASNSceneBase* Scene);
	//! @}
	
	//! @{@name 現在のシーンクラスを取得
	ASNSceneBase* GetCurrentScene();
	//! @}
	
	//! @{@name サウンド管理クラスを取得
	USNSoundManager* GetSoundManager();
	//! @}
	
private:
	
	//!< データアセット管理クラス
	UPROPERTY(EditAnywhere, Category="Data")
	TSoftClassPtr<USNContentAssetManager> ContentAssetManagerClass = nullptr;
	
	//!< オンラインシステムクラス(設定されていればオンラインシステムが有効になります)
	UPROPERTY(EditAnywhere, Category="Online")
	TSoftClassPtr<USNOnlineSystem> OnlineSystemClass = nullptr;
	
	//!< オンラインセッションクラス
	UPROPERTY(EditAnywhere, Category="Online")
	TSubclassOf<UOnlineSession> OnlineSessionClass = USNOnlineSession::StaticClass();
	
	//!< サウンド管理クラス
	UPROPERTY(EditAnywhere, Category="Online")
	TSubclassOf<USNSoundManager> SoundManagerClass = USNSoundManager::StaticClass();
	
	//!< データアセット管理クラスのインスタンス
	UPROPERTY()
	TObjectPtr<USNContentAssetManager> ContentAssetManager = nullptr;
	
	//!< オンラインシステムクラスのインスタンス
	UPROPERTY()
	TObjectPtr<USNOnlineSystem> OnlineSystem = nullptr;
	
	//!< カレントのシーンアクター
	UPROPERTY()
	TObjectPtr<ASNSceneBase> CurrentScene = nullptr;
	
	//!< サウンド管理クラスのインスタンス
	UPROPERTY()
	TObjectPtr<USNSoundManager> SoundManager = nullptr;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief データアセット管理クラスのポインタを取得
//
//! @retval データアセット管理クラスのポインタ
//
//----------------------------------------------------------------------//
FORCEINLINE USNContentAssetManager* USNGameInstance::GetContentAssetManager(){
	return ContentAssetManager;
}

//----------------------------------------------------------------------//
//
//! @brief オンラインシステムのインスタンスを取得
//
//! @retval オンラインシステムのインスタンス
//
//----------------------------------------------------------------------//
FORCEINLINE USNOnlineSystem* USNGameInstance::GetOnlineSystem(){
	return OnlineSystem;
}


//----------------------------------------------------------------------//
//
//! @brief オンラインシステムが有効かのチェック
//
//! @retval true : オンラインシステム有効 / false : オンラインシステム無効
//
//----------------------------------------------------------------------//
FORCEINLINE bool USNGameInstance::IsEnabledOnlineSystem() const {
	return (OnlineSystem != nullptr) ? true : false;
} 

//----------------------------------------------------------------------//
//
//! @brief 現在のシーンクラスを設定
//
//! @param Scene 現在のシーンクラス
//
//----------------------------------------------------------------------//
FORCEINLINE void USNGameInstance::SetCurrentScene(ASNSceneBase* Scene){
	CurrentScene = Scene;
}

//----------------------------------------------------------------------//
//
//! @brief 現在のシーンクラスを取得
//
//! @retval 現在のシーンクラス
//
//----------------------------------------------------------------------//
FORCEINLINE ASNSceneBase* USNGameInstance::GetCurrentScene(){
	return CurrentScene;
}

//! @{@name オンラインシステムのインスタンスを取得
SNPLUGIN_API USNOnlineSystem* GetOnlineSystem();
//! @}

//! @{@name サウンド管理クラスのインスタンスを取得
SNPLUGIN_API USNSoundManager* GetSoundManater();
//! @}

//! @{常駐用システム音コンポーネントを取得
SNPLUGIN_API USNAudioComponent* GetCoreSoundComponent();
//! @}
