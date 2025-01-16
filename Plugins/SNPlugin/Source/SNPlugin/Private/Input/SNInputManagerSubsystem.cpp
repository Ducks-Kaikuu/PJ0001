// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SNInputManagerSubsystem.h"
#include "SNDef.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//! @param Collection 
//
//----------------------------------------------------------------------//
void USNInputManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

//----------------------------------------------------------------------//
//
//! @brief インプットコンテキストを登録
//
//! @param Name                登録名
//! @param InputMappingContext インプットマッピングコンテキスト
//
//----------------------------------------------------------------------//
void USNInputManagerSubsystem::AddInputContext(const FName& Name, UInputMappingContext* InputMappingContext){
	
	InputMapContextMap.Add(Name, InputMappingContext);
	
	SNPLUGIN_LOG(TEXT("[USNInputManagerSubsystem] : Add Input Context.[%s]"), *Name.ToString());
}

//----------------------------------------------------------------------//
//
//! @brief インプットコンテキストを削除
//
//! @param Name 登録名
//
//----------------------------------------------------------------------//
void USNInputManagerSubsystem::RemoveInputContext(const FName& Name){
	// マップに存在するかチェック
	if(IsExist(Name) == true){
		// マップから削除
		InputMapContextMap.Remove(Name);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 全てのインプットマッピングコンテキストを削除
//
//----------------------------------------------------------------------//
void USNInputManagerSubsystem::RemoveAllInputContext(APlayerController* PlayerController){
	// nullチェック
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
	
	for(auto& InputContext : InputMapContextMap){
		// インプットマッピングコンテキストを無効化
		DisableInputMapping(PlayerController, InputContext.Key);
	}
	// マップを全てクリア
	InputMapContextMap.Empty();
	
	SNPLUGIN_LOG(TEXT("[USNInputManagerSubsystem] : Remove All Input Context.[%s]"), *PlayerController->GetName());
}

//----------------------------------------------------------------------//
//
//! @brief インプットマッピングコンテキストを有効化
//
//! @param Name          登録名
//! @param bClearMapping 有効になっているインプットマッピングコンテキストをクリアするか
//
//----------------------------------------------------------------------//
bool USNInputManagerSubsystem::EnableInputMapping(APlayerController* PlayerController, const FName& Name, bool bClearMapping){
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
	// マップに登録されているかチェック
	if(InputMapContextMap.Contains(Name) == false){
		
		SNPLUGIN_LOG(TEXT("InputMappingContext can't Find.[%s]"), *Name.ToString());
		
		return false;
	}
	
	bool Result = false;
	
	ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
	// リモートの
	if(LocalPlayer != nullptr){
		// サブシステムを取得
		UEnhancedInputLocalPlayerSubsystem* Subsystem(LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>());
		
		SNPLUGIN_ASSERT(Subsystem != nullptr, TEXT("Input Subsystem is nullptr."));
		
		if(bClearMapping == true){
			// すでに登録されているマッピングコンテキストをクリア
			Subsystem->ClearAllMappings();
		}
		// インプットマッピングコンテキストを取得
		if(UInputMappingContext* IMC = InputMapContextMap[Name]){
			
			if(UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings()){
				// ユーザー設定に登録
				Settings->RegisterInputMappingContext(IMC);
			}
			// オプションを定義
			FModifyContextOptions Options = {};
			// @@Satoshi Nishimura 2024/10/07
			// これを設定すると即座に切り替わらなかったため、一旦コメント
			//Options.bIgnoreAllPressedKeysUntilRelease = false;
			// マッピングコンテキストを登録
			Subsystem->AddMappingContext(IMC, 0, Options);
			
			SNPLUGIN_LOG(TEXT("EnableInput Success. [%s] "), *Name.ToString());
			
			Result = true;
		}
		
	} else {
		SNPLUGIN_LOG(TEXT("EnableInput Failed.Because LocalPlayer is nullptr.[%s] "), *Name.ToString());
	}
	
	return Result;
}

//----------------------------------------------------------------------//
//
//! @brief インプットマッピングコンテキストを無効化
//
//! @param PlayerController プレイヤーコントローラ
//! @param Name             登録名
//
//----------------------------------------------------------------------//
bool USNInputManagerSubsystem::DisableInputMapping(APlayerController* PlayerController, const FName& Name){
	
	SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
	// マップに登録されているかチェック
	if(InputMapContextMap.Contains(Name) == false){
		
		SNPLUGIN_LOG(TEXT("InputMappingContext can't Find.[%s]"), *Name.ToString());
		
		return false;
	}
	
	bool Result = false;
	
	ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
	
	if(LocalPlayer != nullptr){
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem(LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>());
		
		if(Subsystem != nullptr){
			
			if(UInputMappingContext* IMC = InputMapContextMap[Name]){
				
				if(UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings()){
					// ユーザー設定から登録を削除
					Settings->UnregisterInputMappingContext(IMC);
				}
				
				FModifyContextOptions Options = {};
				// マッピングコンテキストを削除
				Subsystem->RemoveMappingContext(IMC, Options);
				
				SNPLUGIN_LOG(TEXT("DisableInput Success. [%s] "), *Name.ToString());
				
				Result = true;
			}
		}
	} else {
		SNPLUGIN_LOG(TEXT("DeisbleInput Failed.Because LocalPlayer is nullptr.[%s] "), *Name.ToString());
	}
	
	return Result;
}
