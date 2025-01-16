// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNOnlineSystemCommonV1.h"
#include "SNOnlineSystemNull.generated.h"

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief プラットフォーム設定がNULLの場合の通信処理
//
//! @note NULLの場合、同じLAN内のみマッチング可能。ロビーなどは使用不可。
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNOnlineSystemNull : public USNOnlineSystemCommonV1
{
	GENERATED_BODY()

};
//! @}
//! @}
