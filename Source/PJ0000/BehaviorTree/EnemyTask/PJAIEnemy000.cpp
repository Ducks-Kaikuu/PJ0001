// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"

#include "AI/EQS/Utility/SNEqsLocationList.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Base/SNPlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APJAIEnemy000::APJAIEnemy000(const FObjectInitializer& ObjectInitializer)
{
	BehaviorComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));

	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
}

void APJAIEnemy000::SetPlayerKey(APawn* PlayerPawn)
{
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsObject(PlayerKeyName, PlayerPawn);
	}
}

ASNPlayerBase* APJAIEnemy000::GetPlayerKey()
{
	if (BlackboardComponent != nullptr)
	{
		ASNPlayerBase* PlayerPawn = Cast<ASNPlayerBase>(BlackboardComponent->GetValueAsObject(PlayerKeyName));
		
		return PlayerPawn; 
	}

	return nullptr;
}

USNEqsLocationList* APJAIEnemy000::GetEqsLocationList()
{
	USNEqsLocationList* LocationList = nullptr;
	
	if (BlackboardComponent != nullptr)
	{
		UObject* Object = BlackboardComponent->GetValueAsObject(LocationName);
		
		LocationList = Cast<USNEqsLocationList>(BlackboardComponent->GetValueAsObject(LocationName));
	}

	return LocationList;
}

void APJAIEnemy000::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if ((BlackboardComponent != nullptr) && (BehaviorComponent != nullptr))
	{
		if (BehaviorTree != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

			BehaviorComponent->StartTree(*BehaviorTree);
		}
	}
}

void APJAIEnemy000::OnUnPossess()
{
	Super::OnUnPossess();

	if (BehaviorComponent != nullptr)
	{
		BehaviorComponent->StopTree();
	}
}

void APJAIEnemy000::BeginPlay()
{
	Super::BeginPlay();

	EqsLocationList = NewObject<USNEqsLocationList>();

	if (EqsLocationList != nullptr)
	{
		if (BlackboardComponent != nullptr)
		{
			BlackboardComponent->SetValueAsObject(LocationName, EqsLocationList);
		}
	}
}

bool APJAIEnemy000::IsBehaviorTreeRunning() const
{
	if(BehaviorComponent != nullptr)
	{
		return BehaviorComponent->IsRunning();
	}

	return false;
}

void APJAIEnemy000::Restart()
{
	if(BehaviorComponent != nullptr)
	{
		BehaviorComponent->RestartTree();
	}
}

void APJAIEnemy000::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITORONLY_DATA

	if (bEqsDebugDraw == true)
	{
		float Duration = GetActorTickInterval();

		const TArray<FVector4>& Locations(EqsLocationList->GetLocations());

		int Index = 0;
		
		for (auto& Location : Locations)
		{
			float Score = Location.W;

			FLinearColor Color = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
		
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location, 50.0f, 3, Color, Duration, 5.0f);
		}
	}
#endif
}
