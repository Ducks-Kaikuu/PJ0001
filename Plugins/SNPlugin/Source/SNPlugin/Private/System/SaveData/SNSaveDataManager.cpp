// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SaveData/SNSaveDataManager.h"

#include "GameDelegates.h"
#include "Kismet/GameplayStatics.h"
#include "System/SaveData/SNSaveData.h"
#include "System/SaveData/SNSaveGameObject.h"

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#define SN_SYSTEM_SAVE_DATA_NAME	("SystemData")
#define SN_GAME_DATA_NAME			("GameData")
#define SN_AUTOSAVE_DATA_NAME		("AutoSaveData")
#define SN_USER_INDEX				(0)

//----------------------------------------------------------------------//
//
//! @brief スロット毎のセーブを非同期で行うクラス
//
//----------------------------------------------------------------------//
class SNPLUGIN_API FSNGameDataAsync : public FNonAbandonableTask{
	
	friend class FAutoDeleteAsyncTask<FSNGameDataAsync>;
	
	//------------------------------------------------------------------//
	//
	//! @brief デフォルトコンストラクタ
	//
	//! @param InWork ジョブ
	//! @param slot   スロット番号
	//
	//------------------------------------------------------------------//
	FSNGameDataAsync(TFunction<void(int slot)> InWork, int Slot):
	 SlotNo(Slot)
	,Function(InWork)
	{
		
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief ジョブの実行
	//
	//------------------------------------------------------------------//
	void DoWork(){
		Function(SlotNo);
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief スレッドIDの取得
	//
	//! @retval スレッドID
	//
	//------------------------------------------------------------------//
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAFGameDataAsync, STATGROUP_ThreadPoolAsyncTasks);
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief デストラクタ
	//
	//------------------------------------------------------------------//
	~FSNGameDataAsync(){}
	
private:
	
	//!< @brief スロット番号
	int			SlotNo = 0;
	
	//!< @brief ジョブ
	TFunction<void(int slot)> Function;
};

//----------------------------------------------------------------------//
//
//! @brief スロットに関係なくセーブを非同期で行うクラス
//
//----------------------------------------------------------------------//
class SNPLUGIN_API FSNSystemDataAsync : public FNonAbandonableTask{
	
	friend class FAutoDeleteAsyncTask<FSNSystemDataAsync>;
	
	//------------------------------------------------------------------//
	//
	//! @brief デフォルトコンストラクタ
	//
	//! @param InWork ジョブ
	//
	//------------------------------------------------------------------//
	explicit FSNSystemDataAsync(const TFunction<void()>& InWork):
	Function(InWork)
	{
		
	}

	FSNSystemDataAsync(const FSNSystemDataAsync& Rhs):Function(Rhs.Function)
	{
		
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief ジョブの実行
	//
	//------------------------------------------------------------------//
	void DoWork(){
		Function();
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief スレッドIDの取得
	//
	//! @retval スレッドID
	//
	//------------------------------------------------------------------//
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAFSystemDataAsync, STATGROUP_ThreadPoolAsyncTasks);
	}
	
	//------------------------------------------------------------------//
	//
	//! @brief デストラクタ
	//
	//------------------------------------------------------------------//
	~FSNSystemDataAsync(){}
	
private:
	
	//!< @brief ジョブ
	TFunction<void()> Function;
};

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNSaveDataManager::USNSaveDataManager():
 Super()
,SaveData(nullptr)
,SaveObject(nullptr)
,SaveSlotNo(-1)
,bAccessData(false)
{
	
}

//----------------------------------------------------------------------//
//
//! @brief ロード後に走る処理
//
//----------------------------------------------------------------------//
void USNSaveDataManager::PostLoad(){
	
	Super::PostLoad();

	SNPLUGIN_ASSERT(SaveDataClass.IsNull(), TEXT("[USNSaveDataManager::PostLoad] : Save Data Class is not set."));
	// セーブデータクラスをロード
	UObject* Object = SaveDataClass.LoadSynchronous();
	// クラス情報にキャスト
	UClass* Class = Cast<UClass>(Object);

	SNPLUGIN_ASSERT(Class != nullptr, TEXT("[USNSaveDataManager::PostLoad] : Failed to load SaveData Class."));
	
	// セーブデータオブジェクトを生成
	SaveData = NewObject<USNSaveData>(Class);
	// ロードに失敗したらアサート
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data Can't Create"));
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//----------------------------------------------------------------------//
void	USNSaveDataManager::Initialize(){
	
	SNPLUGIN_ASSERT(SaveData != nullptr, TEXT("Save Data Can't Create"));
	// セーブデータの初期化
	SaveData->Init();
	// セーブ用のセーブオブジェクトを生成
	SaveObject = Cast<USNSaveGameObject>(UGameplayStatics::CreateSaveGameObject(USNSaveGameObject::StaticClass()));
}

//----------------------------------------------------------------------//
//
//! @brief 終了処理
//
//----------------------------------------------------------------------//
void	USNSaveDataManager::Deinitialize(){
	
}

//----------------------------------------------------------------------//
//
//! @brief 同期でゲームデータのセーブを実行
//
//! @param Slot      スロット番号
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveGameDataSync(int Slot){

	// セーブ開始時のデリゲートを実行
	if (OnSaveGameDataStart.IsBound())
	{
		OnSaveGameDataStart.Broadcast();		
	}
	// データを取得
	const void* Data = SaveData->GetGameData();
	// ファイル名を生成
	FString Filename(FString::Format(TEXT("{0}_{1}"), {SN_GAME_DATA_NAME, Slot}));
	// セーブ実行
	bool bResult = SaveExecute(Data, sizeof(uint8)*SN_GAMEDATA_MAX_SIZE, Filename);
	// セーブ終了後のデリゲートを実行
	if (OnSaveGameDataFinished.IsBound())
	{
		OnSaveGameDataFinished.Broadcast();
	}
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief 同期でシステムデータのセーブを実行
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveSystemDataSync(){
	// データを取得
	const void* Data = SaveData->GetSystemData();
	// セーブ実行
	bool bResult = SaveExecute(Data, sizeof(uint8)*SN_SYSTEM_DATA_MAX_SIZE, SN_SYSTEM_SAVE_DATA_NAME);

	if (OnSaveSystemDataFinished.IsBound())
	{
		// セーブ終了後のデリゲートを実行
		OnSaveSystemDataFinished.Broadcast();
	}
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief 同期でゲームデータのオートセーブを実行
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveAutoDataSync(){
	// セーブ開始時のデリゲートを実行
	OnAutoSaveStart.Broadcast();
	// データを取得
	const void* Data = SaveData->GetGameData();
	// セーブ実行
	bool bResult = SaveExecute(Data, sizeof(uint8)*SN_GAMEDATA_MAX_SIZE, SN_AUTOSAVE_DATA_NAME);
	// セーブ終了後のデリゲートを実行
	OnAutoSaveFinished.Broadcast();
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief 同期でゲームデータをロード
//
//! @param Slot      スロット番号
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadGameDataSync(int Slot){
	// ファイル名にスロット番号を追加
	FString filename(FString::Format(TEXT("{0}_{1}"), {SN_GAME_DATA_NAME, Slot}));
	// 書き戻しメソッド
	TFunction<void(void*, size_t)> Function = [this](void* pData, size_t size){
		SaveData->CopyGameData(pData, size);
	};
	// ロードを実行
	bool result = LoadExecute(filename, Function);
	// ロード終了後のデリゲートを実行
	if (OnLoadGameDataFinished.IsBound())
	{
		OnLoadGameDataFinished.Broadcast();	
	}
		
	return result;
}

//----------------------------------------------------------------------//
//
//! @brief 同期でシステムデータをロード
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadSystemDataSync(){
	// 書き戻しメソッド
	TFunction<void(void*, size_t)> Function = [this](void* pData, size_t size) {
		SaveData->CopySystemData(pData, size);
	};
	// ロードを実行
	bool bResult = LoadExecute(SN_SYSTEM_SAVE_DATA_NAME, Function);
	// ロード終了後のデリゲートを実行
	if (OnLoadSystemDataFinished.IsBound())
	{
		OnLoadSystemDataFinished.Broadcast();
	}
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief 同期でオートセーブデータをロード
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadAutoDataSync(){
	
	TFunction<void(void*, size_t)> Function = [this](void* pData, size_t size) {
		SaveData->CopyGameData(pData, size);
	};
	// ロードを実行
	bool bResult = LoadExecute(SN_AUTOSAVE_DATA_NAME, Function);
	// ロード終了後のデリゲートを実行
	if (OnLoadAutoSaveDataFinished.IsBound())
	{
		OnLoadAutoSaveDataFinished.Broadcast();
	}
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でゲームデータをセーブ
//
//! @param Slot スロット番号
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveGameDataAsync(int Slot){
	
	TFunction<void(int)> Function = [this](int Slot){
		SaveGameDataSync(Slot);
	};
	// 非同期でデータにアクセス
	AccessGameDataAsync(Function, Slot);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でシステムデータをセーブ
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveSystemDataAsync(){
	
	TFunction<void()> Function = [this](){
		SaveSystemDataSync();
	};
	// 非同期でデータにアクセス
	AccessSystemDataAsync(Function);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でオートセーブデータをセーブ
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveAutoDataAsync(){
	
	TFunction<void()> Function = [this](){
		SaveAutoDataSync();
	};
	// 非同期でデータにアクセス
	AccessSystemDataAsync(Function);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でゲームデータをロード
//
//! @param slot スロット番号
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadGameDataAsync(int Slot){
	
	TFunction<void(int)> Function = [this](int Slot){
		LoadGameDataSync(Slot);
	};
	// 非同期でデータにアクセス
	AccessGameDataAsync(Function, Slot);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でゲームデータをロード
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadSystemDataAsync(){
	
	TFunction<void()> Function = [this](){
		LoadSystemDataSync();
	};
	// 非同期でデータにアクセス
	AccessSystemDataAsync(Function);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でオートセーブデータをロード
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadAutoDataAsync(){
	
	TFunction<void()> Function = [this](){
		LoadAutoDataSync();
	};
	// 非同期でデータにアクセス
	AccessSystemDataAsync(Function);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でゲームデータへアクセス
//
//! @param function ジョブ
//! @param slot     スロット番号
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::AccessGameDataAsync(const TFunction<void(int)>& Function, int Slot){
	// 非同期ジョブ実行クラスを生成
	auto AsyncTask = new FAutoDeleteAsyncTask<FSNGameDataAsync>(
		Function,
		Slot
	);
	// ジョブの実行
	AsyncTask->StartBackgroundTask(GThreadPool, EQueuedWorkPriority::High);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief 非同期でシステム/オートセーブデータへアクセス
//
//! @param function ジョブ
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::AccessSystemDataAsync(const TFunction<void()>& Function){
	// 非同期ジョブ実行クラスを生成
	auto AsyncTask = new FAutoDeleteAsyncTask<FSNSystemDataAsync>(
		Function
	);
	// ジョブの実行
	AsyncTask->StartBackgroundTask(GThreadPool, EQueuedWorkPriority::High);
	
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief セーブ実行処理
//
//! @param pSaveData セーブするデータ
//! @param Size      データのサイズ
//! @param Name      ファイル名
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::SaveExecute(const void* Data, size_t Size, const FString& Name){
	
	if(IsValidLowLevel() == false){
		return false;
	}
	
	uint64 userIndex = SN_USER_INDEX;
	
	SNPLUGIN_ASSERT(bAccessData == false, TEXT("Access to save data while saving"));
	
	bAccessData = true;
	
	SNPLUGIN_ASSERT(SaveObject != nullptr, TEXT("Save Object Is nullptr"));
	SNPLUGIN_ASSERT(SaveData   != nullptr, TEXT("Save Data Is nullptr"));
	
	FScopeLock SaveDataScope(&SaveDataMutex);
	// セーブ終了後のデリゲートを登録
	FGameDelegates::Get().GetExtendedSaveGameInfoDelegate().BindUObject(this, &USNSaveDataManager::SetSaveSettingDelegate);
	// セーブデータを生成
	SaveObject->Write(Data, Size);
	// セーブ実行
	bool bResult = SaveObject->SaveGame(Name, userIndex);
	
	bAccessData = false;
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief ロード実行処理
//
//! @param Name     ファイル名
//! @param Function データのコピージョブ
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool	USNSaveDataManager::LoadExecute(const FString& Name, const TFunction<void(void*, size_t)>& Function){
	
	if(this->IsValidLowLevel() == false){
		return false;
	}
	
	uint64 UserIndex = SN_USER_INDEX;
	
	SNPLUGIN_ASSERT(bAccessData == false, TEXT("Access to save data while loading"));
	
	bAccessData = true;
	
	SNPLUGIN_ASSERT(SaveObject != nullptr, TEXT("Save Object Is nullptr"));
	SNPLUGIN_ASSERT	(SaveData   != nullptr, TEXT("Save Data Is nullptr"));
	
	FScopeLock SaveDataScope(&SaveDataMutex);
	// データをロード
	bool bResult = SaveObject->LoadGame(Name, UserIndex);
	
	if(bResult == true){
		// データの内容尾書き戻し
		bResult = SaveObject->WriteBack(Function);
	}
	
	bAccessData = false;
	
	return bResult;
}

//----------------------------------------------------------------------//
//
//! @brief セーブ後の設定用デリゲート
//
//! @param SaveName セーブを実行したファイル名
//! @param Key      設定のキー情報
//! @param Output   出力文字列
//
//----------------------------------------------------------------------//
void	USNSaveDataManager::SetSaveSettingDelegate(const TCHAR* SaveName, const EGameDelegates_SaveGame Key, FString& Output){
	
	if(Key == EGameDelegates_SaveGame::Title){
		
		FString name(SaveName);
		
		if(SaveName == FString(SN_SYSTEM_SAVE_DATA_NAME)){
			Output = TEXT("ARMED FANTASIA SYSTEM DATA");
		} else 
		if(SaveName == FString(SN_AUTOSAVE_DATA_NAME)){
			Output = TEXT("ARMED FANTASIA AUTO SAVE DATA");
		} else {
			Output = FString::Format(TEXT("ARMED FANTASIA : SLOT{0}"), {SaveSlotNo+1});
		}
	} else
	if(Key == EGameDelegates_SaveGame::MaxSize){
		Output = FString::FromInt(SN_GAMEDATA_MAX_SIZE);
	}
}
