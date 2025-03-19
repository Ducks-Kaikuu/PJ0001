// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SaveData/SNSaveData.h"

#include "SNDef.h"

#if !SN_DEBUG_SAVE_DATA
//----------------------------------------------------------------------//
//
//! @brief サイズによるオフセット情報の更新処理
//
//! @param Map データマップへの参照
//
//----------------------------------------------------------------------//
void	RecalcOffsetBySize(TMap<FName, FSNSaveDataInfo>& Map){
	
	int Count = 0;
	
	uint64 Offset = 0;
	
	for(auto& Info:Map){
		
		if(Count == 0){
			Info.Value.Offset = 0;
		} else {
			// オフセットを更新
			Info.Value.Offset = Offset;
		}
		
		Info.Value.Reserved = 0;
		// オフセットに予約領域分を追加
		Offset += Info.Value.Size;
		
		++Count;
	}
}
#endif

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNSaveData::USNSaveData(){
	
}

//----------------------------------------------------------------------//
//
//! @brief ロード後に走る処理
//
//----------------------------------------------------------------------//
void USNSaveData::PostLoad(){
	
	Super::PostLoad();
	
	// デバッグ用のデータではない場合
#if !SN_DEBUG_SAVE_DATA
	RecalcOffsetBySize(SystemMap);
	RecalcOffsetBySize(GameMap  );
#endif
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//----------------------------------------------------------------------//
void	USNSaveData::Init(){
	// 0で初期化
	memset(SystemData, 0, sizeof(uint8)*SN_SYSTEM_DATA_MAX_SIZE);
	memset(GameData  , 0, sizeof(uint8)*SN_GAMEDATA_MAX_SIZE   );
}

//----------------------------------------------------------------------//
//
//! @brief データの先頭ポインタを取得
//
//! @param InfoMap    マップ情報
//! @param Name       データ名
//! @param Data      実データの先頭アドレス
//
//! @retval 指定されたデータの先頭ポインタ
//
//----------------------------------------------------------------------//
void*	USNSaveData::GetData(const TMap<FName, FSNSaveDataInfo>& InfoMap, const FName& Name, uint8* Data, size_t Size) const {
	// マップ情報に存在しない場合はnullを返す
	if(InfoMap.Find(Name) == nullptr){
		return nullptr;
	}
	// データ情報を取得
	const FSNSaveDataInfo& Info(InfoMap[Name]);
	
	SNPLUGIN_ASSERT(Info.Size > Size, TEXT("Data Size is over\n"));
	// 先頭へのポインタを取得
	void* Top = &Data[Info.Offset];
	
	return Top;
}

//----------------------------------------------------------------------//
//
//! @brief ゲームデータをコピー
//
//! @param Data コピー元のデータ
//! @param Size  データサイズ
//
//----------------------------------------------------------------------//
void	USNSaveData::CopyGameData(void* Data, size_t Size){
	// サイズチェック
	SNPLUGIN_ASSERT(Size == sizeof(uint8)*SN_GAMEDATA_MAX_SIZE, TEXT("Data Size is Invalidate"));
	// データをコピー
	memcpy(GameData, Data, sizeof(uint8)*SN_GAMEDATA_MAX_SIZE);
}

//----------------------------------------------------------------------//
//
//! @brief システムデータをコピー
//
//! @param pData コピー元のデータ
//! @param size  データサイズ
//
//----------------------------------------------------------------------//
void	USNSaveData::CopySystemData(void* Data, size_t Size){
	// サイズチェック
	SNPLUGIN_ASSERT(Size == sizeof(uint8)*SN_SYSTEM_DATA_MAX_SIZE, TEXT("Data Size is Invalidate"));
	// データをコピー
	memcpy(SystemData, Data, sizeof(uint8)*SN_SYSTEM_DATA_MAX_SIZE);
}

#if WITH_EDITOR
//----------------------------------------------------------------------//
//
//! @brief プロパティが変更された際の処理
//
//! @param PropertyChangedEvent 変更内容
//
//----------------------------------------------------------------------//
void USNSaveData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent){
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if(PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet){
		
		const FName MemberName(PropertyChangedEvent.MemberProperty->GetFName());
		// とりあえずシステムデータとゲームデータのみ
		if(MemberName.ToString() != TEXT("SystemMap")
		&& MemberName.ToString() != TEXT("GameMap"  )){
			return;
		}
		
		const FName PropName(PropertyChangedEvent.GetPropertyName());
		// 変更がsizeじゃない場合は終了
		if(PropName.ToString() != TEXT("Size")){
			return;
		}
		// マップへのポインタを取得
		TMap<FName, FSNSaveDataInfo>* Map = (MemberName.ToString() == TEXT("m_systemMap")) ? &SystemMap : &GameMap;
		
		bool bUpdate = false;
		
		size_t Totalsize = 0, Reservesize = 0;
		
		for(auto& Info:*Map){
			// 予約領域が設定されていない場合は新規追加とみなし、最初のsizeの倍を設定
			if(Info.Value.Reserved == 0){
				
				Info.Value.Reserved = Info.Value.Size * 2;
				
				bUpdate = true;
			}
			
			if(Info.Value.Size > Info.Value.Reserved){
				// エラーメッセージを表示
				FString Msg = FString::Printf(TEXT("[%s] : データのサイズが予約領域を超えました。\n %d バイトに抑える必要があります。\n%d バイトに抑える場合はYesを押してください。\n無理な場合はNoを押してください。\nNoを押した場合はセーブデータが使えなくなるので全体に周知をお願いします" ), *Info.Key.ToString(), Info.Value.Reserved, Info.Value.Reserved);
				
				EAppReturnType::Type Type = FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(Msg));
				
				if(Type == EAppReturnType::Yes){
					// データサイズを調整
					Info.Value.Size = Info.Value.Reserved;
				} else {
					// 予約領域を大きくする
					Info.Value.Reserved = Info.Value.Size * 2;
					
					bUpdate = true;
				}
			}
			
			Reservesize += Info.Value.Reserved;
			Totalsize   += Info.Value.Size;
		}
		
		if(MemberName.ToString() == TEXT("m_systemMap")){
			
			if(Reservesize >= SN_SYSTEM_DATA_MAX_SIZE){
				// エラーメッセージを表示
				FString Msg = FString::Printf(TEXT("[%s] : セーブデータのサイズが最大サイズを超えました。[%d]\n MGR 西村宛に連絡をしてください。このまま実行すると不具合になります。" ), *MemberName.ToString(), Reservesize);
				
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Msg));
			}
		} else {
			
			if(Reservesize >= SN_GAMEDATA_MAX_SIZE){
				// エラーメッセージを表示
				FString Msg = FString::Printf(TEXT("[%s] : セーブデータのサイズが最大サイズを超えました。[%d]\n MGR 西村宛に連絡をしてください。このまま実行すると不具合になります。" ), *MemberName.ToString(), Reservesize);
				
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Msg));
			}
		}
		// オフセットの更新が必要な場合はオフセット更新
		if(bUpdate == true){
			
			int Count = 0;
			
			uint64 Offset = 0;
			
			for(auto& Info:*Map){
				
				if(Count == 0){
					Info.Value.Offset = 0;
				} else {
					// オフセットを更新
					Info.Value.Offset = Offset;
				}
				// オフセットに予約領域分を追加
				Offset += Info.Value.Reserved;
				
				++Count;
			}
		}
	}
}
#endif

