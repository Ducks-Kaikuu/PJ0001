// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SNSoundManager.h"

#include "SNDef.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SNAudioComponent.h"
#include "Sound/SoundCue.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNSoundManager::USNSoundManager(){
	CoreSoundComponent = CreateDefaultSubobject<USNAudioComponent>(TEXT("CoreSoundComponent"));
}

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//----------------------------------------------------------------------//
void USNSoundManager::Initialize(){
	// ワールドが破棄された際に呼ばれるデリゲートを設定
	GEngine->OnWorldDestroyed().AddUObject(this, &USNSoundManager::OnWorldDestroyed);
	// ワールド内で管理しているリストを初期化
	InWorldAudioComponents.Empty();
	// ワールドまたいで管理しているリストを初期化
	AcrossLevelAudioComponents.Empty();
}

//----------------------------------------------------------------------//
//
//! @brief ボリュームを設定
//
//! @param Handle サウンドハンドル
//! @param Volume ボリューム値(0 - 1)
//
//----------------------------------------------------------------------//
void USNSoundManager::SetVolume(const FSNSoundHandle& Handle, float Volume){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->SetVolumeMultiplier(Volume);
	}
}

//----------------------------------------------------------------------//
//
//! @brief ピッチを設定
//
//! @param Handle サウンドハンドル
//! @param Pitch  ピッチ値(0 - 1)
//
//----------------------------------------------------------------------//
void USNSoundManager::SetPitch(const FSNSoundHandle& Handle, float Pitch){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->SetPitchMultiplier(Pitch);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 停止処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNSoundManager::Stop(const FSNSoundHandle& Handle){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->Stop();
	}
}

//----------------------------------------------------------------------//
//
//! @brief 一時停止処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNSoundManager::Pause(const FSNSoundHandle& Handle){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->SetPaused(true);
	}
}

//----------------------------------------------------------------------//
//
//! @brief レジューム処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNSoundManager::Resume(const FSNSoundHandle& Handle){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->SetPaused(false);
	}
}

//----------------------------------------------------------------------//
//
//! @brief フェードイン処理
//
//! @param Handle    サウンドハンドル
//! @param InTime    フェード時間
//! @param StartTime 再生を開始する時間
//
//----------------------------------------------------------------------//
void USNSoundManager::FadeIn(const FSNSoundHandle& Handle, float InTime, float StartTime){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->FadeIn(InTime, 1.0f, StartTime);
	}
}

//----------------------------------------------------------------------//
//
//! @brief フェードアウト処理
//
//! @param Handle サウンドハンドル
//! @param InTime フェード時間
//
//----------------------------------------------------------------------//
void USNSoundManager::FadeOut(const FSNSoundHandle& Handle, float InTime){
	// オーディオコンポーネントを取得
	UAudioComponent* AudioComponent(GetAudioComponent(Handle));
	
	if(AudioComponent != nullptr){
		AudioComponent->FadeOut(InTime, 1.0f);
	}
}

//----------------------------------------------------------------------//
//
//! @brief オーディオコンポーネントを取得
//
//! @param Handle サウンドハンドル
//
//! @retval オーディオコンポーネント
//
//----------------------------------------------------------------------//
UAudioComponent* USNSoundManager::GetAudioComponent(const FSNSoundHandle& Handle){
	
	if(InWorldAudioComponents.Contains(Handle)){
		return InWorldAudioComponents[Handle];
	}
	
	if(AcrossLevelAudioComponents.Contains(Handle)){
		return AcrossLevelAudioComponents[Handle];
	}
	
	return nullptr;
}

//----------------------------------------------------------------------//
//
//! @brief 2Dサウンドを再生
//
//! @param Cue          サウンドキュー
//! @param bAcrossLevel レベルをまたいで再生するか
//! @param bLoop        ループするか
//! @param Volume       ボリューム値
//! @param Pitch        ピッチ値
//! @param StartTime    再生開始時間
//
//! @retval サウンドハンドル
//
//----------------------------------------------------------------------//
FSNSoundHandle USNSoundManager::PlaySound2D(USoundCue* Cue, bool bAcrossLevel, bool bLoop, float Volume, float Pitch, float StartTime){
	
	FSNSoundHandle Guid;
	// ワールドを取得
	UWorld* World = GetPrimaryWorld();
	// オーディオコンポーネントを生成
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(World, Cast<USoundBase>(Cue), Volume, Pitch, StartTime, nullptr, bAcrossLevel);
	
	if(AudioComponent != nullptr){
		
		if(bLoop == true){
			// ループ｡｡｡できないの｡｡｡？
			AudioComponent->ActiveCount = 100000;	
		}
		// 終了した際のデリゲートを登録
		AudioComponent->OnAudioFinishedNative.AddUObject(this, &USNSoundManager::OnAudioFinished);
		// ハンドルを生成
		Guid = FSNSoundHandle::GenerateSoundHandle();
		// リストに登録
		if(bAcrossLevel == true){
			AcrossLevelAudioComponents.Add(Guid, AudioComponent);
		} else {
			InWorldAudioComponents.Add(Guid, AudioComponent);
		}
	}
	
	return Guid;
}

//----------------------------------------------------------------------//
//
//! @brief 3Dサウンドを再生
//
//! @param Cue       サウンドキュー
//! @param Position  再生位置
//! @param Rotator   再生角度
//! @param Volume    ボリューム値
//! @param Pitch     ピッチ値
//! @param StartTime 再生開始時間
//
//! @retval サウンドハンドル
//
//----------------------------------------------------------------------//
FSNSoundHandle USNSoundManager::PlaySound3D(USoundCue* Cue, FVector Position, FRotator Rotator, float Volume, float Pitch, float StartTime){
	
	FSNSoundHandle Guid;
	// ワールドを取得
	UWorld* World = GetPrimaryWorld();
	// オーディオコンポーネントを生成
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(World, Cast<USoundBase>(Cue), Position, Rotator, Volume, Pitch, StartTime, SoundAttenuation);
	
	if(AudioComponent != nullptr){
		// 終了した際のデリゲートを登録
		AudioComponent->OnAudioFinishedNative.AddUObject(this, &USNSoundManager::OnAudioFinished);
		// ハンドルを生成
		Guid = FSNSoundHandle::GenerateSoundHandle();
		// リストに登録
		InWorldAudioComponents.Add(Guid, AudioComponent);
	}
	
	return Guid;
}

//----------------------------------------------------------------------//
//
//! @brief 指定されたコンポーネントにアタッチして音を再生
//
//! @param Cue        サウンドキュー
//! @param Parent     アタッチするコンポーネント
//! @param SocketName アタッチするソケット
//! @param Offset     オフセット情報
//! @param Rotator    ローテーション情報
//! @param Volume     ボリューム値
//! @param Pitch      ピッチ値
//! @param StartTime  再生開始時間
//
//! @retval サウンドハンドル
//
//----------------------------------------------------------------------//
FSNSoundHandle USNSoundManager::PlaySoundAttatched(USoundCue* Cue, USceneComponent* Parent, const FName SocketName, FVector Offset, FRotator Rotator, float Volume, float Pitch, float StartTime){
	
	FSNSoundHandle Guid;
	// オーディオコンポーネントを生成
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(Cast<USoundBase>(Cue), Parent, SocketName, Offset, Rotator, EAttachLocation::Type::KeepRelativeOffset, false, Volume, Pitch, StartTime);
	
	if(AudioComponent != nullptr){
		
		if(Parent != nullptr){
			// 親のコンポーネントが無効化された際のデリゲートを登録
			Parent->OnComponentDeactivated.AddDynamic(this, &USNSoundManager::OnParentComponentDeactivate);
		}
		// 終了した際のデリゲートを登録
		AudioComponent->OnAudioFinishedNative.AddUObject(this, &USNSoundManager::OnAudioFinished);
		// ハンドルを生成
		Guid = FSNSoundHandle::GenerateSoundHandle();
		// リストに登録
		InWorldAudioComponents.Add(Guid, AudioComponent);
	}
	
	return Guid;
}

//----------------------------------------------------------------------//
//
//! @brief 指定されたオーディオコンポーネントを破棄
//
//! @param Component オーディオコンポーネント
//
//----------------------------------------------------------------------//
void USNSoundManager::RemoveAudioComponent(UAudioComponent* Component){
	// nullチェック
	if(Component == nullptr){
		
		SNPLUGIN_LOG(TEXT("RemoveAudioComponent : AudioComponent is null!"));
		
		return;
	}
	// レベルを跨いで再生しているかチェック
	if(Component->bIgnoreForFlushing == true){
		// キーを検索
		const uint32* Guid(AcrossLevelAudioComponents.FindKey(Component));
		
		if(Guid != nullptr){
			AcrossLevelAudioComponents.Remove(*Guid);
		} else {
			SNPLUGIN_LOG(TEXT("RemoveAudioComponent : %s is not Managed in AcrossLevel!"), *Component->Sound.GetName());
		}
	} else {
		// キーを検索
		const uint32* Guid(InWorldAudioComponents.FindKey(Component));
		
		if(Guid != nullptr){
			InWorldAudioComponents.Remove(*Guid);
		} else {
			SNPLUGIN_LOG(TEXT("RemoveAudioComponent : %s is not Managed in InWorld!"), *Component->Sound.GetName());
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief 再生が終了した際に呼ばれるデリゲート
//
//! @param AudioComponent 再生が終了したコンポーネント
//
//----------------------------------------------------------------------//
void USNSoundManager::OnAudioFinished(UAudioComponent* AudioComponent){
	RemoveAudioComponent(AudioComponent);
}

//----------------------------------------------------------------------//
//
//! @brief レベルが破棄された際に呼ばれるデリゲート
//
//! @param World 破棄されたレベル
//
//----------------------------------------------------------------------//
void USNSoundManager::OnWorldDestroyed(UWorld* World){
	// レベルをまたがないオーディオコンポーネントを削除
	InWorldAudioComponents.Empty();
	
	SNPLUGIN_LOG(TEXT("SoundManager : Success World Destroy."));
}


//----------------------------------------------------------------------//
//
//! @brief 親が無効化した際に呼ばれるデリゲート
//
//! @param Component 
//
//----------------------------------------------------------------------//
void USNSoundManager::OnParentComponentDeactivate(UActorComponent* Component){
	
	USceneComponent* Parent(Cast<USceneComponent>(Component));
	
	if(Parent != nullptr){
		// アタッチされている子コンポーネントを取得
		TArray<TObjectPtr<USceneComponent>> Children(Parent->GetAttachChildren());
		
		for(auto Child : Children){
			// オーディオコンポーネントへキャスト
			UAudioComponent* AudioComponent = Cast<UAudioComponent>(Child);
			
			if(AudioComponent != nullptr){
				// コンポーネントを削除
				RemoveAudioComponent(Cast<UAudioComponent>(AudioComponent));
			}
		}
	}
}
