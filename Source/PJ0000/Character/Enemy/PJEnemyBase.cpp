// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/PJEnemyBase.h"

#include "PJEnemyGroup.h"
#include "PJEnemyManager.h"
#include "BehaviorTree/EnemyTask/PJAIEnemy000.h"
#include "Character/PJCharacterDef.h"
#include "System/PJGameInstance.h"
#include "Utility/SNUtility.h"

APJEnemyBase::APJEnemyBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer){
	
}

const APJAIEnemy000* APJEnemyBase::GetAIController() const
{
	const APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	return AIController;
}

APJAIEnemy000* APJEnemyBase::GetAIController()
{
	APJAIEnemy000* AIController = Cast<APJAIEnemy000>(GetController());

	return AIController;
}

void APJEnemyBase::SetTeamID(const FGenericTeamId& TeamID)
{
	APJAIEnemy000* AIController = GetAIController();

	if (AIController != nullptr)
	{
		AIController->SetGenericTeamId(TeamID);
	}
}

FGenericTeamId APJEnemyBase::GetTeamID() const
{
	const APJAIEnemy000* AIController = GetAIController();

	if (AIController != nullptr)
	{
		return AIController->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

void APJEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	APJAIEnemy000* AIController = GetAIController();

	if (AIController != nullptr)
	{
		// 初期値を設定。
		AIController->SetGenericTeamId(static_cast<uint8>(ETeamID::Enemy_AdJoin));
	}

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance != nullptr)
	{
		GameInstance->GetEnemyManager()->AddEnemy(this);
	}
}

void APJEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance != nullptr)
	{
		GameInstance->GetEnemyManager()->RemoveEnemy(this);
	}

	if (GroupManager != nullptr)
	{
		GroupManager->RemoveMember(this);

		GroupManager = nullptr;
	}
}
