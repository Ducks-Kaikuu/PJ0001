// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SNAudioComponent.h"
#include "System/SNGameInstance.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNAudioComponent::USNAudioComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetComponentTickEnabled(false);
	// ...
}

//----------------------------------------------------------------------//
//
//! @brief 2Dサウンドを再生
//
//! @param SoundName    サウンド名
//! @param bAcrossLevel レベルをまたいで再生するか
//! @param bLoop        ループするか
//! @param Volume       ボリューム値
//! @param Pitch        ピッチ値
//! @param StartTime    再生開始時間
//
//! @retval サウンドハンドル
//
//----------------------------------------------------------------------//
FSNSoundHandle USNAudioComponent::PlaySound2D(const FName& SoundName, bool bAcrossLevel, bool bLoop, float Volume, float Pitch, float StartTime) const {
	
	FSNSoundHandle Result;
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if((SoundManager != nullptr) && (SoundList.Contains(SoundName) == true)){
		Result = SoundManager->PlaySound2D(SoundList[SoundName], bAcrossLevel, bLoop, Volume, Pitch, StartTime);
	}
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief 3Dサウンドを再生
//
//! @param SoundName サウンド名
//! @param Position  再生位置
//! @param Rotator   再生角度
//! @param Volume    ボリューム値
//! @param Pitch     ピッチ値
//! @param StartTime 再生開始時間
//
//! @retval サウンドハンドル
//
//----------------------------------------------------------------------//
FSNSoundHandle USNAudioComponent::PlaySound3D(const FName& SoundName, FVector Position, FRotator Rotator, float Volume,	float Pitch, float StartTime) const {
	
	FSNSoundHandle Result;
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if((SoundManager != nullptr) && (SoundList.Contains(SoundName) == true)){
		Result = SoundManager->PlaySound3D(SoundList[SoundName], Position, Rotator, Volume, Pitch, StartTime);
	}
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief 指定されたコンポーネントにアタッチして音を再生
//
//! @param SoundName  サウンド名
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
FSNSoundHandle USNAudioComponent::PlaySoundAttatched(const FName& SoundName, USceneComponent* Parent, const FName SocketName, FVector Offset, FRotator Rotator, float Volume, float Pitch, float StartTime) const {
	
	FSNSoundHandle Result;
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if((SoundManager != nullptr) && (SoundList.Contains(SoundName) == true)){
		Result = SoundManager->PlaySoundAttatched(SoundList[SoundName], Parent, SocketName, Offset, Rotator, Volume, Pitch, StartTime);
	}
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief ボリュームを設定
//
//! @param Handle サウンドハンドル
//! @param Volume ボリューム値(0 - 1)
//
//----------------------------------------------------------------------//
void USNAudioComponent::SetVolume(const FSNSoundHandle& Handle, float Volume){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->SetVolume(Handle, Volume);
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
void USNAudioComponent::SetPitch(const FSNSoundHandle& Handle, float Pitch){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->SetPitch(Handle, Pitch);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 停止処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNAudioComponent::Stop(const FSNSoundHandle& Handle){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->Stop(Handle);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 一時停止処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNAudioComponent::Pause(const FSNSoundHandle& Handle){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->Pause(Handle);
	}
}

//----------------------------------------------------------------------//
//
//! @brief レジューム処理
//
//! @param Handle サウンドハンドル
//
//----------------------------------------------------------------------//
void USNAudioComponent::Resume(const FSNSoundHandle& Handle){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->Resume(Handle);
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
void USNAudioComponent::FadeIn(const FSNSoundHandle& Handle, float InTime, float StartTime){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->FadeIn(Handle, InTime, StartTime);
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
void USNAudioComponent::FadeOut(const FSNSoundHandle& Handle, float InTime){
	
	USNSoundManager* SoundManager(GetSoundManater());
	
	if(SoundManager != nullptr){
		SoundManager->FadeOut(Handle, InTime);
	}
}
