// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDelegates.h"
#include "SNDef.h"
#include "SNSaveData.h"
#include "UObject/NoExportTypes.h"
#include "SNSaveDataManager.generated.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
class USNSaveData;
class USNSaveGameObject;

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameDataStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameDataFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadGameDataFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveSystemDataFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadSystemDataFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAutoSaveStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAutoSaveFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadAutoSaveDataFinished);

//!@{@defgroup セーブデータ
//!@{
//----------------------------------------------------------------------//
//
//! @brief セーブデータ管理クラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNSaveDataManager : public UObject
{
	GENERATED_BODY()
public:
	
	//! @{@name デフォルトコンストラクタ
	USNSaveDataManager();
	//! @}
	
	//! @{@name ロード後の処理
	virtual void PostLoad() override;
	//! @}
	
	//! @{@name 初期化処理
	void	Initialize();
	//! @}
	
	//! @{@name 終了処理
	void Deinitialize();
	//! @}
	
	//! @{@name ゲームデータへのポインタを取得
	template<class T>
	T*		GetGameData(const FName& Name);
	
	template<class T>
	const T*	GetGameData(const FName& Name) const ;
	//! @}
	
	//! @{@name ゲームデータへのポインタを取得
	template<class T>
	T*		GetSystemData(const FName& Name);
	
	template<class T>
	const T*	GetSystemData(const FName& Name) const ;
	//! @}
	
	//! @{@name 同期でゲームデータをセーブ
	bool	SaveGameDataSync(int Slot);
	//! @}
	
	//! @{@name 同期でシステムデータのセーブを実行
	bool	SaveSystemDataSync();
	//! @}
	
	//! @{@name 同期でゲームデータのオートセーブを実行
	bool	SaveAutoDataSync();
	//! @}
	
	//! @{@name 同期でゲームデータをロード
	bool	LoadGameDataSync(int Slot);
	//! @}
	
	//! @{@name 同期でシステムデータをロード
	bool	LoadSystemDataSync();
	//! @}
	
	//! @{@name 同期でオートセーブデータをロード
	bool	LoadAutoDataSync();
	//! @}
	
	//! @{@name 非同期でゲームデータをセーブ
	bool	SaveGameDataAsync(int Slot);
	//! @}
	
	//! @{@name 非同期でゲームデータをセーブ
	bool	SaveSystemDataAsync();
	//! @}
	
	//! @{@name 非同期でゲームデータをセーブ
	bool	SaveAutoDataAsync();
	//! @}
	
	//! @{@name 非同期でゲームデータをロード
	bool	LoadGameDataAsync(int Slot);
	//! @}
	
	//! @{@name 非同期でシステムデータをロード
	bool	LoadSystemDataAsync();
	//! @}
	
	//! @{@name 非同期でオートセーブデータをロード
	bool	LoadAutoDataAsync();
	//! @}

	//!< @brief ゲームデータがセーブ開始した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Save")
	FOnSaveGameDataStart		OnSaveGameDataStart;
	
	//!< @brief ゲームデータがセーブ終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Save")
	FOnSaveGameDataFinished		OnSaveGameDataFinished;
	
	//!< @brief ゲームデータがロード終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Load")
	FOnLoadGameDataFinished		OnLoadGameDataFinished;
	
	//!< @brief システムデータがセーブ終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Save")
	FOnSaveSystemDataFinished	OnSaveSystemDataFinished;
	
	//!< @brief システムデータがロード終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Load")
	FOnLoadSystemDataFinished	OnLoadSystemDataFinished;
	
	//!< @brief ゲームデータのオートセーブが開始した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Save")
	FOnAutoSaveStart			OnAutoSaveStart;
	
	//!< @brief ゲームデータのオートセーブが終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Save")
	FOnAutoSaveFinished			OnAutoSaveFinished;
	
	//!< @brief オートセーブデータのロードが終了した際のデリゲート
	UPROPERTY(BlueprintAssignable, Category = "AF|SaveData|Load")
	FOnLoadAutoSaveDataFinished	OnLoadAutoSaveDataFinished;
	
private:
	
	//! @{@name セーブ実行処理
	bool	SaveExecute(const void* Data, size_t Size, const FString& Name);
	//! @}
	
	//! @{@name ロード実行処理
	bool	LoadExecute(const FString& Name, const TFunction<void(void*, size_t)>& Function);
	//! @}
	
	//! @{@name 非同期でゲームデータへアクセス
	bool	AccessGameDataAsync(const TFunction<void(int)>& Function, int Slot);
	//! @}
	
	//! @{@name 非同期でシステム/オートセーブデータへアクセス
	bool	AccessSystemDataAsync(const TFunction<void()>& Function);
	//! @}
	
	void	SetSaveSettingDelegate(const TCHAR* SaveName, const EGameDelegates_SaveGame Key, FString& Output);

	//!< @brief セーブデータクラス
	UPROPERTY(EditAnywhere, Meta=(DisplayName="Save Data Class"), Category=SaveData)
	TSoftClassPtr<USNSaveData> SaveDataClass;
	
	//!< @brief セーブデータの内容
	UPROPERTY()
	TObjectPtr<USNSaveData> SaveData;
	
	//!< @brief セーブ用オブジェクト
	UPROPERTY()
	TObjectPtr<USNSaveGameObject> SaveObject;
	
	//!< @brief セーブ中のスロット番号
	int		SaveSlotNo;
	
	//!< @brief データへのアクセスフラグ
	bool	bAccessData;
	
	//!< @brief ミューテックス
	FCriticalSection SaveDataMutex;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief ゲームデータへのポインタを取得
//
//! @param Name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE T*	USNSaveDataManager::GetGameData(const FName& Name){
	
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data is nullptr"));
	
	return SaveData->GetGameData<T>(Name);
}

//----------------------------------------------------------------------//
//
//! @brief ゲームデータへのポインタを取得
//
//! @param Name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE const T*	USNSaveDataManager::GetGameData(const FName& Name) const {
	
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data is nullptr"));
	
	return SaveData->GetGameData<T>(Name);
}

//----------------------------------------------------------------------//
//
//! @brief システムデータへのポインタを取得
//
//! @param Name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE T*	USNSaveDataManager::GetSystemData(const FName& Name){
	
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data is nullptr"));
	
	return SaveData->GetSystemData<T>(Name);
}

//----------------------------------------------------------------------//
//
//! @brief システムデータへのポインタを取得
//
//! @param Name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE const T*	USNSaveDataManager::GetSystemData(const FName& Name) const {
	
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data is nullptr"));
	
	return SaveData->GetSystemData<T>(Name);
}
