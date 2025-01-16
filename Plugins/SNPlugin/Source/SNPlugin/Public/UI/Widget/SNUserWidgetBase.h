// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/SNUIDef.h"
#include "SNUserWidgetBase.generated.h"

class USNMasterWidget;
//!@{@defgroup UI
//!@{
//----------------------------------------------------------------------//
//
//! @brief ユーザーウィジェットのベースクラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
	friend class USNMasterWidget;
	
public:
	
	//! @{@name 登録しているレイヤーを取得
	EWidgetLayer GetRegistLayer() const ;
	//! @}
	
private:
	
	//! @{@name 登録しているレイヤーを設定
	void SetRegistLayer(EWidgetLayer Layer);
	//! @}
	
	//!< 登録しているレイヤー情報
	EWidgetLayer RegistLayer = EWidgetLayer::Invalid;
};

//----------------------------------------------------------------------//
//
//! @brief 登録しているレイヤーを取得
//
//! @retval 登録しているレイヤー
//
//----------------------------------------------------------------------//
FORCEINLINE EWidgetLayer USNUserWidgetBase::GetRegistLayer() const {
	return RegistLayer;
}

//----------------------------------------------------------------------//
//
//! @brief 登録しているレイヤーを設定
//
//! @param Layer レイヤー
//
//----------------------------------------------------------------------//
FORCEINLINE void USNUserWidgetBase::SetRegistLayer(EWidgetLayer Layer){
	RegistLayer = Layer;
}
//! @}
//! @}
