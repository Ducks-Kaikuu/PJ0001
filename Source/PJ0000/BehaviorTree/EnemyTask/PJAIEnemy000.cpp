// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"

#include "AI/EQS/Utility/SNEqsLocationList.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Base/SNPlayerBase.h"

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
