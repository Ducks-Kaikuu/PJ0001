// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SNBlueprintFunctionLibrary.h"

#include "ChooserFunctionLibrary.h"
#include "IObjectChooser.h"
#include "SNDef.h"
#include "GameFramework/Character.h"
#include "Input/SNInputManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Online/SNOnlineSystem.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "PoseSearch/PoseSearchResult.h"
#include "System/SNGameInstance.h"

void USNBlueprintFunctionLibrary::SetInputMappingContext(APlayerController* PlayerController, FName Name, bool bClear)
{
	USNGameInstance* GameInstance(Cast<USNGameInstance>(UGameplayStatics::GetGameInstance(GetPrimaryWorld())));

	if(GameInstance == nullptr)
	{
		SNPLUGIN_LOG(TEXT("Failed to Start Online System because SNGameInstance is nullptr."));

		return;
	}

	USNInputManagerSubsystem* InputManagerSubsystem(GameInstance->GetSubsystem<USNInputManagerSubsystem>());

	if(InputManagerSubsystem != nullptr)
	{
		InputManagerSubsystem->EnableInputMapping(PlayerController, Name, bClear);
	}
}

void USNBlueprintFunctionLibrary::Login()
{
	USNOnlineSystem* OnlineSystem(GetOnlineSystem());

	if(OnlineSystem != nullptr)
	{
		OnlineSystem->Login();	
	}
	
}

void USNBlueprintFunctionLibrary::HostSession()
{
	USNOnlineSystem* OnlineSystem(GetOnlineSystem());

	if(OnlineSystem != nullptr)
	{
		OnlineSystem->HostSession();
	}
}

void USNBlueprintFunctionLibrary::FindSession()
{
	USNOnlineSystem* OnlineSystem(GetOnlineSystem());

	if(OnlineSystem != nullptr)
	{
		OnlineSystem->FindSession();
	}
}

void USNBlueprintFunctionLibrary::KillSession(FName SessionName)
{
	USNOnlineSystem* OnlineSystem(GetOnlineSystem());

	if(OnlineSystem != nullptr)
	{
		OnlineSystem->KillSession(SessionName);
	}
}

USNOnlineSystem* USNBlueprintFunctionLibrary::GetOnlineSystem()
{
	USNGameInstance* GameInstance(Cast<USNGameInstance>(UGameplayStatics::GetGameInstance(GetPrimaryWorld())));

	if(GameInstance == nullptr)
	{
		SNPLUGIN_LOG(TEXT("Failed to get OnlineSyste because SNGameInstance is nullptr."));

		return nullptr;
	}

	if(GameInstance->IsEnabledOnlineSystem() == false)
	{
		SNPLUGIN_LOG(TEXT("Failed to get OnlineSyste because OnlneSystem is nullptr."));

		return nullptr;
	}

	return GameInstance->GetOnlineSystem();
}

UAnimMontage* USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(ACharacter* Character, UChooserTable* ChooserTable, UObject* Parameter, float& PlayRate, float& StartTime)
{
	if(Character == nullptr){
		
		SNPLUGIN_LOG(TEXT("GetAnimMontageFromChooser : Character is nullptr."));
		
		return nullptr;
	}
	
	if(ChooserTable == nullptr){
		
		SNPLUGIN_LOG(TEXT("GetAnimMontageFromChooser : ChooserTable is nullptr."));
		
		return nullptr;
	}
	//! チューザーを評価する
	FChooserEvaluationContext Context = UChooserFunctionLibrary::MakeChooserEvaluationContext();
	//! ChooserTable評価用の構造体を設定(ChooserTable側に設定している構造体の型と同じもの)
	Context.AddObjectParam(Parameter);
	
	FInstancedStruct ChooserInstance = UChooserFunctionLibrary::MakeEvaluateChooser(ChooserTable);
	//! 評価を実行
	TArray<UObject*> Assets(UChooserFunctionLibrary::EvaluateObjectChooserBaseMulti(Context, ChooserInstance, UAnimMontage::StaticClass()));
	
	if(Assets.Num() <= 0){
		
		SNPLUGIN_WARNING(TEXT("Motion is not find."));
		
		return nullptr;
	}
	UAnimInstance* AnimInstanceBase = Cast<UAnimInstance>(Character->GetMesh()->GetAnimInstance());
	
	FPoseSearchBlueprintResult PoseSearchResult;
	//! 選択されたモンタージュから最良のモンタージュを検索
	UPoseSearchLibrary::MotionMatch(AnimInstanceBase, Assets, FName(TEXT("PoseHistory")), FPoseSearchContinuingProperties(), FPoseSearchFutureProperties(), PoseSearchResult);
	
	
	PlayRate = PoseSearchResult.WantedPlayRate;
	
	StartTime = PoseSearchResult.SelectedTime;
	
	const UAnimMontage* AnimMontage = nullptr;
	
	if(PoseSearchResult.SelectedAnimation != nullptr){
		AnimMontage = Cast<UAnimMontage>(PoseSearchResult.SelectedAnimation);
	} else {
		AnimMontage = Cast<UAnimMontage>(Assets[0]);
		
		SNPLUGIN_LOG(TEXT("Motion is not find in Database"));
	}
	
	return const_cast<UAnimMontage*>(AnimMontage);
}

