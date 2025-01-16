// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "SNGameSession.generated.h"

//!@{@defgroup オンライン
//!@{
//----------------------------------------------------------------------//
//
//! @brief ゲームセッションクラス
//
//! @note ホストのみに存在します。
//
//----------------------------------------------------------------------//
UCLASS(Blueprintable)
class SNPLUGIN_API ASNGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	
	ASNGameSession(const FObjectInitializer& ObjectInitializer);
	
protected:
	
	//! @{@name GameModeのInitGameからオートログインのタイミングで呼ばれます。
	virtual bool ProcessAutoLogin() override;
	//! @}
	
	//! @{@name ワールドのBeginplayから呼ばれます。
	virtual void HandleMatchHasStarted() override;
	//! @}
	
	//! @{@name GameModeのEndMatchが呼ばれ、ステートがWaitingPostMatchに変わった際に呼ばれます。
	virtual void HandleMatchHasEnded() override;
	//! @}
};
//! @}
//! @}

