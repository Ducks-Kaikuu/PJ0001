// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNSoundHandle.generated.h"

//!@{@defgroup サウンド
//!@{
//----------------------------------------------------------------------//
//
//! @brief サウンドハンドル
//
//----------------------------------------------------------------------//
USTRUCT(BlueprintType)
struct SNPLUGIN_API FSNSoundHandle
{
	GENERATED_BODY()

	friend class USNSoundManager;
	
	//!< 無効値
	static const FSNSoundHandle Invalid;
	
	//! @{@name デフォルトコンストラクタ
	FSNSoundHandle();
	//! @}
	
	//! @{@name コピーコンストラクタ
	FSNSoundHandle(const FSNSoundHandle& other);
	//! @}
	
	//! @{@name uint32への暗黙的変換
	operator uint32() const ;
	//! @}
	
	//! @{@name 代入演算子
	FSNSoundHandle& operator=(const FSNSoundHandle& other);
	//! @}
	
	//! @{@name 等式比較演算子
	bool operator==(const FSNSoundHandle& other) const ;
	bool operator==(uint32 rhs) const ;
	//! @}
	
	//! @{@name 非等式比較演算子
	bool operator!=(const FSNSoundHandle& other) const ;
	bool operator!=(uint32 rhs) const ;
	//! @}
	
	//! @{@name ハンドルが有効かチェック
	bool IsValid() const ;
	//! @}
	
private:
	
	//! @{@name サウンドハンドルを発行
	static FSNSoundHandle GenerateSoundHandle();
	//! @}
	
	//! @{@name デフォルトコンストラクタ
	FSNSoundHandle(uint32 handle);
	//! @}
	
	//! @{@name 代入演算子
	FSNSoundHandle& operator=(uint32 rhs);
	//! @}
	
	//!< ハンドル(ユニークID)
	uint32 Handle = Invalid;
};
//! @}
//! @}
