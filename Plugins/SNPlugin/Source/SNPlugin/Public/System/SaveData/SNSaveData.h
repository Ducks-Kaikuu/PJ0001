// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNSaveData.generated.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
class USNSaveDataManager;

// @note ここの定義をエディタから調整できるように出来ないか...。

//----------------------------------------------------------------------//
//! @brief システムセーブデータの最大サイズ									//
//! 案件によって要変更														//
//----------------------------------------------------------------------//
#define SN_SYSTEM_DATA_MAX_SIZE		(10*1024*1024)

//----------------------------------------------------------------------//
//! @brief ゲーム用のセーブデータ最大サイズ
//----------------------------------------------------------------------//
#define SN_GAMEDATA_MAX_SIZE		(30*1024*1024)

//----------------------------------------------------------------------//
//
//! @brief セーブデータのマップ情報
//
//----------------------------------------------------------------------//
USTRUCT(BlueprintType)
struct FSNSaveDataInfo{
	
	GENERATED_USTRUCT_BODY()
	
	//!< @brief データサイズ
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Data Size"), Category = SaveData)
	uint64 Size = 0;
	
	//!< @brief 予約領域へのオフセット
	UPROPERTY()
	uint64 Reserved = 0;
	
	//!< @brief 実データへのオフセット
	UPROPERTY()
	uint64 Offset = 0;
};

//----------------------------------------------------------------------//
//
//! @brief セーブデータクラス
//
//----------------------------------------------------------------------//
UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class SNPLUGIN_API USNSaveData : public UObject
{
	GENERATED_BODY()

	//! @{@name デフォルトコンストラクタ
	USNSaveData();
	//! @}
	
public:

	friend class USNSaveDataManager;
	
	//! @{@name ロード後の処理
	virtual void PostLoad() override;
	//! @}
	
	//! @{@name 初期化処理
	void	Init();
	//! @}
	
#if WITH_EDITOR
	//! @{@name プロパティ変更後のイベント
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//! @}
#endif
	
	//! @{@name ゲームデータへのポインタを取得
	template<class T>
	T*			GetGameData(const FName& Name);
	
	template<class T>
	const T*	GetGameData(const FName& Name) const ;
	//! @}
	
	//! @{@name ゲームデータへのポインタを取得
	template<class T>
	T*			GetSystemData(const FName& Name);
	
	template<class T>
	const T*	GetSystemData(const FName& Name) const ;
	//! @}
	
	
private:
	
	//! @{@name ゲームデータへのポインタを取得
	const void*	GetGameData() const ;
	//! @}
	
	//! @{@name システムデータへのポインタを取得
	const void* GetSystemData() const ;
	//! @}
	
	//! @{@name ゲームデータをコピー
	void	CopyGameData(void* Data, size_t Size);
	//! @}
	
	//! @{@name システムデータをコピー
	void	CopySystemData(void* Data, size_t Size);
	//! @}
	
	//! @{@name データへのポインタを取得
	void*	GetData(const TMap<FName, FSNSaveDataInfo>& InfoMap, const FName& Name, uint8* Data, size_t Size)const ;
	//! @}
	
	//!< @brief システムデータのマップ情報
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "System Data Map"), Category = "SystemData")
	TMap<FName, FSNSaveDataInfo>	SystemMap;
	
	//!< @brief ゲームデータのマップ情報
	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Game Data Map"), Category = "GameData")
	TMap<FName, FSNSaveDataInfo>	GameMap;
	
	//!< @brief システムデータの実データ
	uint8	SystemData[SN_SYSTEM_DATA_MAX_SIZE];
	
	//!< @brief ゲームデータの実データ
	uint8	GameData[SN_GAMEDATA_MAX_SIZE];
};

//----------------------------------------------------------------------//
//
//! @brief ゲームデータへのポインタを取得
//
//! @param name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE T*	USNSaveData::GetGameData(const FName& Name){
	return (T*)GetData(GameMap, Name, GameData, sizeof(T));
}

//----------------------------------------------------------------------//
//
//! @brief ゲームデータへのポインタを取得
//
//! @param name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE const T*	USNSaveData::GetGameData(const FName& Name) const {
	return (const T*)GetData(GameMap, Name, const_cast<uint8*>(GameData), sizeof(T));
}

//----------------------------------------------------------------------//
//
//! @brief システムデータへのポインタを取得
//
//! @param name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE T*	USNSaveData::GetSystemData(const FName& Name){
	return (T*)GetData(SystemMap, Name, SystemData, sizeof(T));
}

//----------------------------------------------------------------------//
//
//! @brief システムデータへのポインタを取得
//
//! @param name データ名
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE const T*	USNSaveData::GetSystemData(const FName& Name) const {
	return (const T*)GetData(SystemMap, Name, const_cast<uint8*>(SystemData), sizeof(T));
}

//----------------------------------------------------------------------//
//
//! @brief ゲームデータの先頭ポインタを取得
//
//! @retval ゲームデータの先頭ポインタ
//
//----------------------------------------------------------------------//
FORCEINLINE const void* USNSaveData::GetGameData() const {
	return GameData;
}
	
//----------------------------------------------------------------------//
//
//! @brief システムデータの先頭ポインタを取得
//
//! @retval システムデータの先頭ポインタ
//
//----------------------------------------------------------------------//
FORCEINLINE const void* USNSaveData::GetSystemData() const {
	return SystemData;
}

