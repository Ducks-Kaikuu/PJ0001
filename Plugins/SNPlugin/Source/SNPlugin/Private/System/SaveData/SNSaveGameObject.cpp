// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SaveData/SNSaveGameObject.h"

#include "PlatformFeatures.h"
#include "SaveGameSystem.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"

#if PLATFORM_WINDOWS
#include "ThirdParty/OpenSSL/1.1.1t/include/Win64/VS2015/openssl/aes.h"
#elif PLATFORM_PS5
// 参考: OpenSSL_PS5.Build.cs
#include "openssl/aes.h"
#elif PLATFORM_MAC
#include "ThirdParty/OpenSSL/1.0.2g/include/Mac/openssl/aes.h"
#endif

// このnamespaceの中の定義は適宜変更してください。
namespace
{

#define AES_KEYLENGTH 256
	
//----------------------------------------------------------------------//
// 暗号化キー																//
//----------------------------------------------------------------------//
#define AES_KEY_CODE ("MPJNfFkCByyD3AAysxA7kFbWZFNe2h2E")

//----------------------------------------------------------------------//
// ハッシュキー															//
//----------------------------------------------------------------------//
#define AES_HASH_KEY00 (9564953743443338)
#define AES_HASH_KEY01 (9493866868897948)

struct SaveHeader{
	size_t Datasize;
	size_t HashLen;
};
}

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNSaveGameObject::USNSaveGameObject():
 Super()
{
  SaveData.Empty();	
}

//----------------------------------------------------------------------//
//
//! @brief 終了処理
//
//----------------------------------------------------------------------//
void USNSaveGameObject::FinishDestroy()
{
	Super::FinishDestroy();

	if (EncryptData != nullptr)
	{
		delete [] EncryptData;

		EncryptData = nullptr;
	}

	SaveData.Empty();

	DataSize = 0;
}

//----------------------------------------------------------------------//
//
//! @brief セーブデータ書き込み領域へ書き込み
//
//! @param Data セーブするデータへのポインタ
//! @param Size  データサイズ
//
//----------------------------------------------------------------------//
void USNSaveGameObject::Write(const void* Data, int64_t Size)
{
	// データの中身をクリア
	SaveData.Empty();
	// データを暗号化
	uint8* TmpData = Encrypt(Data, Size, DataSize);
	// nullチェック
	if(TmpData == nullptr){
		return;
	}
	// 書き込み用の領域へコピー
	SaveData = TArray<uint8>(TmpData, DataSize);

}

//----------------------------------------------------------------------//
//
//! @brief ゲーム用領域への書き戻し処理
//
//! @param Function 実際に書き戻しを行う関数
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNSaveGameObject::WriteBack(TFunction<void(void*, size_t)> Function)
{
	void* Data = SaveData.GetData();
	// nullチェック
	if(Data == nullptr){
		return false;
	}
	
	uint8* Result	= nullptr;
	
	size_t size		= 0;
	// データを復号化
	bool Ret = Decrypt(Data, SaveData.Num(), Result, size);
	
	if(Ret == true){
		// ゲームで参照する領域に書き戻し
		Function(Result, size);
	}
	
	return Ret;
}

//----------------------------------------------------------------------//
//
//! @brief データのセーブ処理
//
//! @param Filename  ファイル名
//! @param UserIndex ユーザーインデックス
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNSaveGameObject::SaveGame(const FString& Filename, uint64_t UserIndex)
{
	ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();
	
	bool bResult = false;
	
	if(SaveSystem != nullptr){
		bResult = SaveSystem->SaveGame(false, *Filename, UserIndex, SaveData);
	}
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief データのロード処理
//
//! @param Filename  ファイル名
//! @param UserIndex ユーザーインデックス
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNSaveGameObject::LoadGame(const FString& Filename, uint64_t UserIndex)
{
	ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();
	
	bool result = false;
	
	if(SaveSystem != nullptr){
		result = SaveSystem->LoadGame(false, *Filename, UserIndex, SaveData);
	}
	
	return result;
}

//----------------------------------------------------------------------//
//
//! @brief 暗号化処理
//
//! @param Data    データへのポインタ
//! @param Size    データサイズ
//! @param OutSize 暗号化後のデータサイズ
//
//! @retval 暗号化後のバッファ
//
//----------------------------------------------------------------------//
uint8* USNSaveGameObject::Encrypt(const void* Data, size_t Size, size_t& OutSize)
{
	if (Data == nullptr)
	{
		OutSize = 0;

		return nullptr;
	}

	const FString KeyCode(AES_KEY_CODE);
	// 生のポインタを取得
	const TCHAR* Tmp = KeyCode.GetCharArray().GetData();

	uint8* Key = new uint8[AES_KEYLENGTH];
	// キーコードをコピー
	strcpy_s((char*)Key, AES_KEYLENGTH, (char*)(TCHAR_TO_UTF8(Tmp)));
	// ブロックサイズを算出
	uint32 BlockSize = Size + (AES_BLOCK_SIZE - (Size % AES_BLOCK_SIZE));
	
	uint8* TmpData = new uint8[BlockSize];
	
	if(TmpData != nullptr)
	{
		// データをコピー
		memcpy_s((char*)TmpData, BlockSize, (const char*)Data, Size);
		
		int padding = BlockSize - Size;
		// パディングで初期化
		memset(TmpData + Size, padding, padding);
		
		uint8* OutData = new uint8[BlockSize];
		
		if(OutData != nullptr){
			
			memset(OutData, 0, BlockSize);
			
			uint8 initVector[AES_BLOCK_SIZE];
			
			memset(initVector, 0, AES_BLOCK_SIZE);
			
			AES_KEY AESKey;
			
			AES_set_encrypt_key(Key, AES_KEYLENGTH, &AESKey);
			
			AES_cbc_encrypt(TmpData, OutData, BlockSize, &AESKey, initVector, AES_ENCRYPT);
			// バッファがすでに存在する場合は一度破棄
			if(EncryptData != nullptr){
				
				delete [] EncryptData;
				
				EncryptData = nullptr;
			}
			// データの後ろにEngine側のデータが設定されているようなので、最終から32バイトは見ない
			uint64 Hash = CityHash64WithSeeds((const char*)Data, Size, AES_HASH_KEY00, AES_HASH_KEY01);
			
			size_t Headersize = (sizeof(SaveHeader) + sizeof(uint64) + 0xf)&~0xf;
			
			size_t Encryptsize = Headersize + BlockSize;
			
			DataSize = Encryptsize;
			// 最終的に書き込むバッファを確保
			EncryptData = new uint8[Encryptsize];
			
			if(EncryptData != nullptr){
				
				SaveHeader* pHeader = (SaveHeader*)(EncryptData);
				// データサイズとハッシュ値のサイズを設定
				pHeader->Datasize	= Size;
				pHeader->HashLen	= sizeof(uint64);
				// データへ書き込み
				memcpy(EncryptData+sizeof(SaveHeader), &Hash, sizeof(uint64));
				// 暗号化後のデータを書き込み
				memcpy(EncryptData+Headersize, OutData, BlockSize);
			}
			
			if(OutData != nullptr){
				
				delete [] OutData;
				
				OutData = nullptr;
			}
		}
	}
	
	if(Key != nullptr){
		
		delete [] Key;
		
		Key = nullptr;
	}
	
	if(TmpData != nullptr){
		
		delete [] TmpData;
		
		TmpData = nullptr;
	}
	
	
	return EncryptData;
}

//----------------------------------------------------------------------//
//
//! @brief データを復号化
//
//! @param Input     暗号化されたデータ
//! @param InputSize 暗号化後のデータサイズ
//! @param Output    出力バッファへのポインタ
//! @param OutSize   復号化のデータサイズ
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNSaveGameObject::Decrypt(const void* Input, size_t InputSize, uint8*& Output, size_t& OutSize)
{
	// 出力を一旦リセット
	Output		= nullptr;
	OutSize	= 0;
	// nullチェック
	if(Input == nullptr){
		return false;
	}
	// ヘッダへのポインタを取得
	SaveHeader* Header = (SaveHeader*)Input;
	// ハッシュ値の結果を取得
	uint64*	HashPtr = (uint64*)(Header+1);
	// ヘッダサイズを算出
	size_t Headersize = ((sizeof(SaveHeader) + sizeof(uint64)) + 0xf)&~0xf;
	// データへのポインタを取得
	void* Data = (uint8*)Input + Headersize;
	// キーコードを設定
	const FString KeyCode(AES_KEY_CODE);
	// 生のポインタを取得
	const TCHAR* Tmp = KeyCode.GetCharArray().GetData();
	
	uint8* Key = new uint8[AES_KEYLENGTH];
	// キーコードをコピー
	strcpy_s((char*)Key, AES_KEYLENGTH, (char*)(TCHAR_TO_UTF8(Tmp)));
	
	uint32 Size = Header->Datasize;
	// ブロックサイズを算出
	uint32 BlockSize = Size + (AES_BLOCK_SIZE - (Size % AES_BLOCK_SIZE));
	
	uint8* TmpData = new uint8[BlockSize];
	
	bool	bResult = true;
	
	if(TmpData != nullptr){
		
		uint8 InitVector[AES_BLOCK_SIZE];
		
		memset(InitVector, 0, AES_BLOCK_SIZE);
		
		AES_KEY AESKey;	
		// 復号化キーを設定
		AES_set_decrypt_key(Key, AES_KEYLENGTH, &AESKey);
		// データを復号化
		AES_cbc_encrypt((const uint8*)Data, TmpData, BlockSize, &AESKey, InitVector, AES_DECRYPT);
		// バッファが存在する場合は一旦破棄
		if(EncryptData != nullptr){
			
			delete [] EncryptData;
			
			EncryptData = nullptr;
		}
		// 復号化バッファを確保
		EncryptData = new uint8 [Size];
		// nullチェック
		if(EncryptData != nullptr){
			// 復号化したデータをコピー
			memcpy(EncryptData, TmpData, Size);
			// 先頭ポインタをコピー
			Output = EncryptData;
			// サイズをコピー
			OutSize = Size;
			// ハッシュを計算
			uint64 HashValue = CityHash64WithSeeds((const char*)Output, OutSize, AES_HASH_KEY00, AES_HASH_KEY01);
			// セーブ前のハッシュと比較して改ざんチェック
			if(HashValue != *HashPtr){
				// ここに入ったら改ざんされている
				bResult = false;
			}
		}
	}
	
	if(TmpData != nullptr){
		
		delete [] TmpData;
		
		TmpData = nullptr;
	}
	
	if(Key != nullptr){
		
		delete [] Key;
		
		Key = nullptr;
	}
	
	return bResult;
}
