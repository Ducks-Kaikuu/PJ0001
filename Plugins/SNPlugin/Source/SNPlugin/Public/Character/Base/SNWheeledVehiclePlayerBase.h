// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNPlayablePawnInterface.h"
#include "Character/Base/SNWheeledVehiclePawnBase.h"
#include "SNWheeledVehiclePlayerBase.generated.h"

class USNInputConfig;
//----------------------------------------------------------------------//
//
//! @brief プレイ可能なビークル用のインターフェイス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNWheeledVehiclePlayerBase : public ASNWheeledVehiclePawnBase, public ISNPlayablePawnInterface
{
	GENERATED_BODY()

public:

	//! @{@name ゲーム開始処理
	virtual void BeginPlay() override;
	//! @}
	
	//! @{@name 終了処理
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//! @}
	
	//! @{@name 入力された際に呼ばれるアクションを追加
	virtual void AddInputAction(const FGameplayTag& Tag, USNActionBase* Action) override;
	//! @}
	
	//! @{@name アクションを取得
	virtual USNActionBase* GetAction(const FGameplayTag& Tag) override;
	//! @}

	//! @{@name 管理しているアクション数を取得
	virtual int GetActionNum() const override;
	//! @}
	
protected:
	
	virtual void NotifyRestarted() override;
	
private:
	//! @{@name 入力からアクション実行用のレプリケーション(サーバー)
	UFUNCTION(Server, Reliable)
	void ExecuteInputAction_OnServer(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type);
	//! @}
	
	//! @{@name 入力からアクション実行用のレプリケーション(クライアント)
	UFUNCTION(NetMulticast, Reliable)
	void ExecuteInputAction_OnMulticast(const FGameplayTag& Tag, const FVector& InputValue, EInputActionValueType Type);
	//! @}
	
	//! @{@name インプットコンフィグの初期化処理
	void InitializeInputConfig();
	//! @}
	
	//!< 入力用コンフィグのリスト
	UPROPERTY(EditAnyWhere, Category="Input")
	TMap<FName, TObjectPtr<USNInputConfig>> InputConfigMap;
	
	//!< 入力アクションリスト
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USNActionBase>> InputActionMap;
};
