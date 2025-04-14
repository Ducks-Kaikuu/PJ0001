// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/PJEqsManager.h"

#include "CborTypes.h"
#include "SNDef.h"
#include "AI/EQS/Utility/SNEqsLocationList.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#if 0
FEqsRequestInstance::FEqsRequestInstance()
{
	OnQueryFinishedEvent = FQueryFinishedSignature::CreateUObject(this, &FEqsRequestInstance::OnQueryFinished);
	
	OnQueryFinishedEvent = FQueryFinishedSignature::CreateLambda([this](TSharedPtr<FEnvQueryResult> EnvQueryResult)
	{
		OnQueryFinished(EnvQueryResult);
	});
}

void FEqsRequestInstance::OnQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	if (EqsLocationList == nullptr)
	{
		SNPLUGIN_WARNING(TEXT("EQS Location List is nullptr."));
		
		return;
	}

	if (EnvQueryResult->IsSuccessful() == false)
	{
		SNPLUGIN_WARNING(TEXT("EQS Query is Failed."));
		
		return;
	}

	TArray<FVector> Points;
	
	EnvQueryResult->GetAllAsLocations(Points);

	int Index = 0;

	for (auto& Location : Points)
	{
		float Score = EnvQueryResult->GetItemScore(Index);;

		EqsLocationList->AddLocation(Score, Location);
		
#if WITH_EDITORONLY_DATA
		
		if (bEqsDebugDraw == true)
		{
			FLinearColor Color = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
		
			UKismetSystemLibrary::DrawDebugSphere(GetPrimaryWorld(), Location, 50.0f, 3, Color, 0.033f, 10.0f);
		}
#endif
		++Index;
	}
}

#else
UEQSRequestInstance::UEQSRequestInstance()
{
	OnQueryFinishedEvent = FQueryFinishedSignature::CreateUObject(this, &UEQSRequestInstance::OnQueryFinished);
}

void UEQSRequestInstance::PostLoad()
{
	UObject::PostLoad();
#if 0
	if (EqsLocationList == nullptr)
	{
		EqsLocationList = NewObject<USNEqsLocationList>(this);
	}
#endif
}

void UEQSRequestInstance::OnQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	if(EqsLocationList == nullptr){
		
		SNPLUGIN_WARNING(TEXT("EQS Location List is nullptr.[%s]"), *GetPathName());
		
		return;
	}
	
	if(EnvQueryResult->IsSuccessful() == false){
		
		SNPLUGIN_WARNING(TEXT("EQS Query is Failed.[%s]"), *GetPathName());
		
		return;
	}
	
	TArray<FVector> Points;
	
	EnvQueryResult->GetAllAsLocations(Points);

	EqsLocationList->ClearLocations();
	
	int Index = 0;

	for (auto& Location : Points)
	{
		float Score = EnvQueryResult->GetItemScore(Index);;

		EqsLocationList->AddLocation(Score, Location);
		
#if WITH_EDITORONLY_DATA
		if (bEqsDebugDraw == true)
		{
			FLinearColor Color = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
		
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location, 50.0f, 3, Color, 0.033f, 5.0f);
		}
#endif
		++Index;
	}

	EqsLocationList->SortLocations();
}
#endif
void UPJEqsManager::Initialize()
{
	if (EqsMap.Num() <= 0)
	{
		return;
	}

	for (auto& Eqs:EqsMap)
	{
		UClass* Class = Eqs.Value.LoadSynchronous();

		if (Class != nullptr)
		{
			UEQSRequestInstance* Inst = NewObject<UEQSRequestInstance>(this, Class);

			if (Inst != nullptr)
			{
				if (Inst->EqsLocationList == nullptr)
				{
					Inst->EqsLocationList = NewObject<USNEqsLocationList>(this, USNEqsLocationList::StaticClass());
				}
				
				EQSInstanceMap.Add(Eqs.Key, Inst);
			}
		}
	}
}

void UPJEqsManager::RunEqs(UWorld* World, const FGameplayTagContainer& EqsTags)
{
	if (World == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("[UPJEqsManager::RunEqs] World is nullptr."));

		return;
	}
	
//	for (auto& Tag : EqsTags)
	{
		for (auto& EQSInstance : EQSInstanceMap)
		{
			if (EQSInstance.Key.MatchesAny(EqsTags))
			{
				EQSInstance.Value->EQSRequest.Execute(*this, nullptr, EQSInstance.Value->OnQueryFinishedEvent);
			}
		}
	}
}

USNEqsLocationList* UPJEqsManager::GetEqsLocationList(const FGameplayTag& EqsTag)
{
	if (EQSInstanceMap.Contains(EqsTag))
	{
		UEQSRequestInstance* EqsInstance(EQSInstanceMap[EqsTag]);

		return EqsInstance->EqsLocationList;
	}

	return nullptr;
}

