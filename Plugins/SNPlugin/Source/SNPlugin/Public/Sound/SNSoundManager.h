// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNSoundHandle.h"
#include "UObject/NoExportTypes.h"
#include "SNSoundManager.generated.h"

class UAudioComponent;
class USoundCue;
class USNAudioComponent;
class USoundAttenuation;

//!@{@defgroup サウンド
//!@{
//----------------------------------------------------------------------//
//
//! @brief サウンド管理クラス
//
//----------------------------------------------------------------------//
UCLASS(Blueprintable)
class SNPLUGIN_API USNSoundManager : public UObject
{
	GENERATED_BODY()
public:
	
	friend class USNAudioComponent;
	
	//! @{@name デフォルトコンストラクタ
	USNSoundManager();
	//! @}
	
	//! @{@name 初期化処理
	virtual void Initialize();
	//! @}
	
	//! @{@name ボリュームを設定
	void SetVolume(const FSNSoundHandle& Handle, float Volume);
	//! @}
	
	//! @{@name ピッチを設定
	void SetPitch(const FSNSoundHandle& Handle, float Pitch);
	//! @}
	
	//! @{@name 停止処理
	void Stop(const FSNSoundHandle& Handle);
	//! @}
	
	//! @{@name 一時停止処理
	void Pause(const FSNSoundHandle& Handle);
	//! @}
	
	//! @{@name レジューム処理
	void Resume(const FSNSoundHandle& Handle);
	//! @}
	
	//! @{@name フェードイン処理
	void FadeIn(const FSNSoundHandle& Handle, float InTime, float StartTime=0.0f);
	//! @}
	
	//! @{@name フェードアウト処理
	void FadeOut(const FSNSoundHandle& Handle, float InTime);
	//! @}
	
	//! @{@name 常駐用システム音コンポーネントを取得
	USNAudioComponent* GetCoreSoundComponent() const ;
	//! @}
	
private:
	
	//! @{@name 再生が終了した際に呼ばれるデリゲート
	void OnAudioFinished(UAudioComponent* AudioComponent);
	//! @}
	
	//! @{@name レベルが破棄された際に呼ばれるデリゲート
	UFUNCTION()
	void OnWorldDestroyed(UWorld* World);
	//! @}
	
	UFUNCTION()
	void OnParentComponentDeactivate(UActorComponent* Component);
	
	//! @{@name オーディオコンポーネントを取得
	UAudioComponent* GetAudioComponent(const FSNSoundHandle& Handle);
	//! @}
	
	//! @{@name 2Dサウンドを再生
	FSNSoundHandle PlaySound2D(USoundCue* Cue, bool bAcrossLevel=false, bool bLoop=false, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f);
	//! @}
	
	//! @{@name 3Dサウンドを再生
	FSNSoundHandle PlaySound3D(USoundCue* Cue, FVector Position, FRotator Rotator=FRotator::ZeroRotator, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f);
	//! @}
	
	//! @{@name 指定されたコンポーネントにアタッチして音を再生
	FSNSoundHandle PlaySoundAttatched(USoundCue* Cue, USceneComponent* Parent, const FName SocketName, FVector Offset=FVector::ZeroVector, FRotator Rotator=FRotator::ZeroRotator, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f);
	//! @}
	
	//! @{@name 指定されたオーディオコンポーネントを破棄
	void RemoveAudioComponent(UAudioComponent* Component);
	//! @}
	
	//!< ワールド内で再生されているオーディオリスト
	UPROPERTY()
	TMap<uint32, TObjectPtr<UAudioComponent>> InWorldAudioComponents;
	
	//!< ワールドをまたいで再生されているオーディオリスト
	UPROPERTY()
	TMap<uint32, TObjectPtr<UAudioComponent>> AcrossLevelAudioComponents;
	
	//!< 常駐のシステム音管理用コンポーネント
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USNAudioComponent> CoreSoundComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundAttenuation> SoundAttenuation = nullptr;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief 常駐用システム音コンポーネントを取得
//
//! @retval 常駐用システム音コンポーネント
//
//----------------------------------------------------------------------//
FORCEINLINE USNAudioComponent* USNSoundManager::GetCoreSoundComponent() const {
	return CoreSoundComponent;
}
