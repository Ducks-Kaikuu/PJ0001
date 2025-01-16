// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "SNPlayablePawnInterface.h"
#include "Character/Base/SNCharacterBase.h"
#include "SNPlayerBase.generated.h"

class USNInputConfig;
//!@{@defgroup キャラクター制御
//!@{
//----------------------------------------------------------------------//
//
//! @brief プレイヤー制御用ベースクラス
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API ASNPlayerBase : public ASNCharacterBase, public ISNPlayablePawnInterface
{
	GENERATED_BODY()
public:

	virtual void PossessedBy(AController* NewController) override;
	
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
	
	//! @{@name ゲーム開始可能状態かチェック
	bool IsReadyToPlay() const ;
	//! @}
	
	//! @{@name 入力の可否を設定
	void SetInputAvailability(bool InAvailability);
	//! @}
	
	template<class T>
	T* GetPlayerController();
	
protected:
	
	virtual void NotifyRestarted() override;
	
private:
	
	//! @{@name BeginPlayがコールされたことをサーバーに通知
	UFUNCTION(Server, Reliable)
	void ReadyToPlayOnServer();
	//! @}
	
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
	
	//!< 現在のインプットタイプ
	//!< 入力アクションリスト
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USNActionBase>> InputActionMap;
	
	//!< 入力が有効化されているかのチェックフラグ
	UPROPERTY()
	bool bPossessed = false;
	
	//!< BeginPlayがコールされたかのチェックフラグ
	UPROPERTY()
	bool bBeginPlay = false;
};
//! @}
//! @}

//----------------------------------------------------------------------//
//
//! @brief ゲーム開始可能状態かチェック
//
//! @retval true : ゲーム開始可能 / false : ゲーム開始不可
//
//----------------------------------------------------------------------//
FORCEINLINE bool ASNPlayerBase::IsReadyToPlay() const {
	return (bPossessed == true) && (bBeginPlay == true);
}

template<class T>
FORCEINLINE T* ASNPlayerBase::GetPlayerController()
{
	return Cast<T>(Controller);
}