// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SNGameSession.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//! @param ObjectInitializer イニシャライザ
//
//----------------------------------------------------------------------//
ASNGameSession::ASNGameSession(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	
}

//----------------------------------------------------------------------//
//
//! @brief GameModeのInitGameからオートログインのタイミングで呼ばれます。
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool ASNGameSession::ProcessAutoLogin(){
	// ProcessAutoLoginをオーバーライドしないとログインに失敗した旨のワーニングが出ます…。
	return true;
}

//----------------------------------------------------------------------//
//
//! @brief ワールドのBeginplayから呼ばれます。
//
//----------------------------------------------------------------------//
void ASNGameSession::HandleMatchHasStarted(){
	Super::HandleMatchHasStarted();
}

//----------------------------------------------------------------------//
//
//! @brief GameModeのEndMatchが呼ばれ、ステートがWaitingPostMatchに変わった際に呼ばれます。
//
//----------------------------------------------------------------------//
void ASNGameSession::HandleMatchHasEnded(){
	Super::HandleMatchHasEnded();
}


