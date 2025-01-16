// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SNSoundManager.h"
#include "SNAudioComponent.generated.h"

class USoundCue;

//!@{@defgroup サウンド
//!@{
//----------------------------------------------------------------------//
//
//! @brief オーディオクラス
//
//----------------------------------------------------------------------//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//! @{@name デフォルトコンストラクタ
	USNAudioComponent();
	//! @}
	
	//! @{@name 2Dサウンドを再生
	FSNSoundHandle PlaySound2D(const FName& SoundName, bool bAcrossLevel=false, bool bLoop=false, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f) const ;
	//! @}
	
	//! @{@name 3Dサウンドを再生
	FSNSoundHandle PlaySound3D(const FName& SoundName, FVector Position, FRotator Rotator=FRotator::ZeroRotator, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f) const ;
	//! @}
	
	//! @{@name 指定されたコンポーネントにアタッチして音を再生
	FSNSoundHandle PlaySoundAttatched(const FName& SoundName, USceneComponent* Parent, const FName SocketName, FVector Offset=FVector::ZeroVector, FRotator Rotator=FRotator::ZeroRotator, float Volume=1.0f, float Pitch=1.0f, float StartTime=0.0f) const ;
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
	
private:
	//!< オーディオリスト
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<USoundCue>> SoundList;
};
//! @}
//! @}
