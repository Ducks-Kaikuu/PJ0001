// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SNSaveGameObject.generated.h"

//!@{@defgroup セーブデータ
//!@{
//----------------------------------------------------------------------//
//
//! @brief セーブデータオブジェクト(セーブデータの実態)
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNSaveGameObject : public USaveGame
{
	GENERATED_BODY()

public:

	//! @{@name デフォルトコンストラクタ
	USNSaveGameObject();
	//! @}
	
	//! @{@name 終了時の処理
	void FinishDestroy() override;
	//! @}

	//! @{@name セーブデータ書き込み領域へ書き込み
	void Write(const void* Data, int64_t Size);
	//! @}
	
	//! @{@name ゲーム用領域への書き戻し処理
	bool	WriteBack(TFunction<void(void*, size_t)> Function);
	//! @}

	//! @{@name データのセーブ処理
	bool SaveGame(const FString& Filename, uint64_t UserIndex);
	//! @}

	//! @{@name データのロード処理
	bool LoadGame(const FString& Filename, uint64_t UserIndex);
	//! @}
	
private:

	//! @{@name 暗号化処理
	uint8* Encrypt(const void* Data, size_t Size, size_t& OutSize);
	//! @}

	//! @{@name 復号化処理
	bool Decrypt(const void* Input, size_t InputSize, uint8*& Output, size_t& OutSize);
	//! @}

	//!< 実際に保存される領域
	UPROPERTY()
	TArray<uint8> SaveData;

	//!< 暗号化後のデータ
	uint8* EncryptData = nullptr;

	//!< 暗号化後のデータサイズ
	size_t DataSize = 0;
};
//! @}
//! @}
