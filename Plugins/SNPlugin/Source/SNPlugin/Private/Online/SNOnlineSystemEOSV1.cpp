// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/SNOnlineSystemEOSV1.h"

#include "Online/SNOnlineSystem.h"
#include "Utility/SNUtility.h"

#if COMMONUSER_OSSV1
#include "SNDef.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/PlayerState.h"
#include "Online/OnlineSessionNames.h"
#include "Online/SNOnlineCommonSetting.h"
#endif

#if COMMONUSER_OSSV1
namespace{
	const FName SETTING_ONLINESUBSYSTEM_VERSION(TEXT("OSSv1"));
}
#endif

//----------------------------------------------------------------------//
//
//! @brief 初期化処理
//
//! @param OnlineSystem オーナーとなるOnlineSystemクラス
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV1::Initialize(USNOnlineSystem* OnlineSystem){
	
	Super::Initialize(OnlineSystem);
	
	UGameInstance* GameInstance(SNUtility::GetGameInstance<UGameInstance>());
	
	if(GameInstance != nullptr){
		// 専用サーバーを持っているかチェック
		bIsDedicatedServer = GameInstance->IsDedicatedServerInstance();
	}
}

//----------------------------------------------------------------------//
//
//! @brief ログイン処理
//
//! @param Identity 認証インターフェイス
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV1::Login(){
#if COMMONUSER_OSSV1
	// オンラインシステムのインターフェイスを取得
	IOnlineSubsystem* OnlineSubsystem(Online::GetSubsystem(GetWorld()));
	
	SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
	
	IOnlineIdentityPtr Identity(OnlineSubsystem->GetIdentityInterface());
	// OnlineIdentityが有効なものかチェック
	if(Identity.IsValid()){
		// プレイヤーコントローラを取得
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		
		SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
		// ローカルプレイヤーを取得
		ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
		
		if(LocalPlayer != nullptr){
			// コントローラIDを取得
			int Id = LocalPlayer->GetControllerId();
			
			if(Identity->GetLoginStatus(Id) != ELoginStatus::LoggedIn){
				
				Identity->AddOnLoginCompleteDelegate_Handle(Id, FOnLoginCompleteDelegate::CreateUObject(this, &USNOnlineSystemEOSV1::OnLoginComplete));
				
#if WITH_EDITORONLY_DATA
				FOnlineAccountCredentials Credentials(TEXT("Developer"), TEXT("localhost:8080"), OwnerOnlineSystem->GetOnlineToken().ToString());
#else
				//FOnlineAccountCredentials Credentials(TEXT("persistentauth"), TEXT(""), TEXT(""));
				FOnlineAccountCredentials Credentials(TEXT("AccountPortal"), TEXT(""), TEXT(""));
				//FOnlineAccountCredentials Credentials(TEXT("Developer"), TEXT("localhost:8080"), TEXT("Satoshi"));
#endif
				SNPLUGIN_WARNING(TEXT("CommandLine : %s"), FCommandLine::Get());
				
				//FName t0;
				//FName t1;
				//FName t2;
				
				//FCommandLine::Set(TEXT("AUTH_TYPE=Developer"));
				//FCommandLine::Set(TEXT("AUTH_LOGIN=localhost:8080"));
				//FCommandLine::Set(TEXT("AUTH_PASSWORD=Satoshi"));
				
				//FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), t0);
				//FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), t1);
				//FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), t2);
				
				
				SNPLUGIN_LOG(TEXT("CommandLine : %s"), FCommandLine::Get());
				
				//Identity->AutoLogin(Id);
				
				Identity->Login(Id, Credentials);
			}
			
			ELoginStatus::Type Status = Identity->GetLoginStatus(Id);
			
			SNPLUGIN_LOG(TEXT("EOS Login Status : %s"), ELoginStatus::ToString(Status));
		}
	}
#endif
}

//----------------------------------------------------------------------//
//
//! @brief ホストとしてセッションを生成
//
//! @param Sessions       セッションインターフェイス
//! @param SessionRequest セッションリクエスト
//! @param Name           セッション名
//
//! @retval true  正常終了
//! @retval false 異常終了
//
//----------------------------------------------------------------------//
bool USNOnlineSystemEOSV1::HostSession(USNOnlineHostSessionRequest* SessionRequest, const FName& Name){
#if COMMONUSER_OSSV1
	
	APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
	
	if(PlayerController != nullptr){
		
		FUniqueNetIdPtr UserId;
		
		ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
		
		// オンラインシステムのインターフェイスを取得
		IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
		
		SNPLUGIN_ASSERT(OnlineSubsystem != nullptr, TEXT("OnlineSubsystem is nullptr"));
		
		if(LocalPlayer != nullptr){
			
			FUniqueNetIdRepl UniqueNetId(LocalPlayer->GetPreferredUniqueNetId());
			
			FName Type(UniqueNetId.GetType());
			
			UserId = UniqueNetId.GetUniqueNetId();
		} else
		if(bIsDedicatedServer){
			UserId = OnlineSubsystem->GetIdentityInterface()->GetUniquePlayerId(0);
		}
		// セッションインターフェイスを取得
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		
		if(Sessions.IsValid()){
			
			Sessions->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &USNOnlineSystemCommonV1::OnCreateSessionComplete));
			
			if(ensure(UserId.IsValid())){
				
				TSharedPtr<class FOnlineSessionSettings> SessionSettings(MakeShareable(new FOnlineSessionSettings()));
				
				SNPLUGIN_ASSERT(SessionSettings != nullptr, TEXT("Failed to allocate Session Settings."));
				// 生成するセッションの設定
				SessionSettings->NumPublicConnections			= SessionRequest->MaxPlayerNum;
				SessionSettings->NumPrivateConnections			= 0;
				SessionSettings->bIsLANMatch					= (SessionRequest->OnlineMode == ESessionOnlineMode::LAN) ? true : false;
				SessionSettings->bShouldAdvertise				= SessionRequest->bShouldIdAdvertise;
				SessionSettings->bAllowJoinInProgress			= SessionRequest->bAllowJoinInProgress;
				SessionSettings->bAllowInvites					= SessionRequest->bAllowInvites;
				SessionSettings->bUsesPresence					= SessionRequest->bUsesPresence;
				SessionSettings->bAllowJoinViaPresence			= SessionRequest->bAllowJoinViaPresense;
				SessionSettings->bUseLobbiesIfAvailable			= SessionRequest->bUseLobbiesIfAvailable;
				SessionSettings->bUseLobbiesVoiceChatIfAvailable= SessionRequest->bUseLobbiesVoiceChatIfAvailable;
				
				SessionSettings->Set(SEARCH_KEYWORDS, Name.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings->Set(SETTING_MATCHING_TIMEOUT, 120.0f, EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings->Set(SETTING_ONLINESUBSYSTEM_VERSION, true, EOnlineDataAdvertisementType::ViaOnlineService);
				
				bool bResult = Sessions->CreateSession(*UserId, Name, *SessionSettings);
				
				if(bResult == true){
					
					SNPLUGIN_LOG(TEXT("Success to Create Session."))
					
					return true;
					
				} else {
					SNPLUGIN_ERROR(TEXT("CreateSession: Fail"));
				}
				
			} else {
				SNPLUGIN_ERROR(TEXT("CreateSession : Unique Net Id is none."))
			}
		}
	}
#endif
	return false;
}

//----------------------------------------------------------------------//
//
//! @brief ログインが完了した際に呼ばれるデリゲート
//
//! @param LocalUserNum   ユーザー数
//! @param bWasSuccessful ログインが完了したかのフラグ
//! @param UserId         ユーザーID
//! @param Error          エラーの内容
//
//----------------------------------------------------------------------//
void USNOnlineSystemEOSV1::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error){
#if COMMONUSER_OSSV1
	
	if(bWasSuccessful == false){
		
		SNPLUGIN_ERROR(TEXT("Login Failed : [%s] : %s"), *UserId.ToString(), *Error);
		
		return;
	}
	
	IOnlineIdentityPtr Identity(Online::GetIdentityInterface(GetWorld()));
	
	if(Identity.IsValid()){
		
		APlayerController* PlayerController(SNUtility::GetPlayerController<APlayerController>());
		
		SNPLUGIN_ASSERT(PlayerController != nullptr, TEXT("PlayerController is nullptr"));
		
		ULocalPlayer* LocalPlayer(PlayerController->GetLocalPlayer());
		
		if(LocalPlayer != nullptr){
			
			int Id = LocalPlayer->GetControllerId();
			
			FUniqueNetIdRepl uniqueId = PlayerController->PlayerState->GetUniqueId();
			
			uniqueId.SetUniqueNetId(FUniqueNetIdWrapper(LocalPlayer->GetCachedUniqueNetId()).GetUniqueNetId());
			
			PlayerController->PlayerState->SetUniqueId(uniqueId);
			
			ELoginStatus::Type Status = Identity->GetLoginStatus(Id);
			
			SNPLUGIN_LOG(TEXT("EOS Login Status : %s"), ELoginStatus::ToString(Status));
		}
	}
	
	if(OwnerOnlineSystem->OnCompleteLoggedIn.IsBound()){
		OwnerOnlineSystem->OnCompleteLoggedIn.Broadcast(bWasSuccessful, Error);
	}
#endif
}
