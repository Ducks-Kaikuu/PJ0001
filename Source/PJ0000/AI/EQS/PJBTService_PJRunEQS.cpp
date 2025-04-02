// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/PJBTService_PJRunEQS.h"

#include "SNDef.h"
#include "VectorUtil.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UPJBTService_PJRunEQS::UPJBTService_PJRunEQS(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	NodeName = TEXT("PJRunEqsQuery");

	QueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UPJBTService_PJRunEQS::OnSNQueryFinished);
}

void UPJBTService_PJRunEQS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	SNPLUGIN_LOG(TEXT("UPJBTService_PJRunEQS::TickNode"));
}

void UPJBTService_PJRunEQS::OnSNQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	TArray<FVector> Points;
	
	EnvQueryResult->GetAllAsLocations(Points);

	int Index = 0;
	
	for (auto& Location : Points)
	{
		if (bDebugDraw == true)
		{
			float Alpha = EnvQueryResult->GetItemScore(Index);
			// スコアを0 - 1の範囲にリマップ
			Alpha = UKismetMathLibrary::MapRangeClamped(Alpha, MinThreshold, MaxThreshold, 0.0f, 1.0f);

			FLinearColor Color = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Alpha);
		
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location, 50.0f, 3, Color, 0.033f, 10.0f);
		}

		++Index;
	}
}
