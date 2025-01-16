// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SNPlayerController.generated.h"

class USNInputConfig;

//!@{@defgroup プレイヤーコントローラ
//!@{
//----------------------------------------------------------------------//
//
//! @brief プレイヤーコントローラベースクラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	//! @{@name 終了処理
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//! @}
	
	//! @{@name 入力の初期化処理
	bool InitializeInput();
	//! @}
	
	//! @{@name 入力タイプを有効化
	UFUNCTION(BlueprintCallable, Category = "SN|Input")
	void EnabledInputType(FName Name, bool bClearAll = true);
	//! @}
	
	//! @{@name 入力タイプを無効化
	void DisableInputType(FName Name);
	//! @}
	
private:
	
	//! @{@name 入力タイプを有効化
	bool SetEnableInput(FName Name, bool bClearAll);
	//! @}
	
	//! @{@name 入力タイプを無効化
	bool SetDisableInput(FName Name);
	//! @}
	
	//!< 現在のインプットタイプ
	UPROPERTY()
	TSet<FName> CurrentInputTypes;
	
	//!< 入力周りの初期化が終了したかどうかのフラグ
	bool bInitializedInput = false;
};
//! @}
//! @}
