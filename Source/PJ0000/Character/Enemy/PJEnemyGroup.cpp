// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/PJEnemyGroup.h"

#include "PJEnemyBase.h"
#include "Developer/Windows/LiveCoding/Private/External/LC_Process.h"
#include "PJ0000/Character/PJCharacterDef.h"

void UPJEnemyGroup::SpawnGroupsEnemy(const TSubclassOf<APJEnemyBase>& EnemyClass, int Num, const FVector& SpawnLocation, float SpawnRadius)
{
	// TeamIDが設定されていない場合は設定
	if (TeamID == FGenericTeamId::NoTeam)
	{
		 TeamID = UPJEnemyGroup::GetTeamId();
	}
	// 敵をスポーン
	for (int i = 0; i < Num; ++i)
	{
		SpawnEnemy(EnemyClass, SpawnLocation, SpawnRadius);
	}
}

FGenericTeamId UPJEnemyGroup::GetTeamId()
{
	return FGenericTeamId(static_cast<uint8>(ETeamID::Enemy_AdJoin));
}

FGameplayTag UPJEnemyGroup::GetEqsName(FGenericTeamId TeamId)
{
	FGameplayTag Result = FGameplayTag::EmptyTag;

	ETeamID ID = static_cast<ETeamID>(TeamId.GetId());
	
	switch (ID)
	{
		case ETeamID::Enemy_AdJoin: Result = FGameplayTag::RequestGameplayTag(TEXT("EQS.Player.Simple")); break;
		case ETeamID::Enemy_Middle: Result = FGameplayTag::RequestGameplayTag(TEXT("EQS.Player.Middle")); break;
		default: break;
	}
	
	return Result;
}

AActor* UPJEnemyGroup::SpawnEnemy(const TSubclassOf<APJEnemyBase>& EnemyClass, const FVector& SpawnLocation, float SpawnRadius)
{
	// SpawnRadiusを半径とする円状の位置を算出(88.0はキャラクターの中心位置の高さ※BP参照)
	FVector Range(FMath::RandRange(0.0f, SpawnRadius * 2.0f) - SpawnRadius , FMath::RandRange(0.0f, SpawnRadius * 2.0f) - SpawnRadius, 88.0f);
	// 250は適当...。中心にスポナーがあるので、半径250cm以内には入らないようにクランプ
	if (Range.X < 0.0f)
	{
		Range.X = FMath::Clamp(Range.X, -SpawnRadius, -250.0f);
	} else
	{
		Range.X = FMath::Clamp(Range.X, 250.0f, SpawnRadius);
	}

	if (Range.Y < 0.0f)
	{
		Range.Y = FMath::Clamp(Range.Y, -SpawnRadius, -250.0f);
	} else
	{
		Range.Y = FMath::Clamp(Range.Y, 250.0f, SpawnRadius);
	}
	// スポーン位置を算出
	FVector Location = SpawnLocation + Range;
	// トランスフォームを生成
	FTransform SpawnTransform = FTransform::Identity;
	// 位置を設定
	SpawnTransform.SetLocation(Location);

	APJEnemyBase* Enemy = Cast<APJEnemyBase>(GetWorld()->SpawnActor(EnemyClass.Get(), &SpawnTransform));

	SetupGroup(Enemy);
		
	return Enemy;
}

void UPJEnemyGroup::SetupGroup(APJEnemyBase* Enemy)
{
	if (Enemy != nullptr)
	{
		Enemy->SetTeamID(TeamID);

		Enemy->SetGroupManager(this);

		Members.Add(Enemy);
	}
}

void UPJEnemyGroup::RemoveMember(APJEnemyBase* Member)
{
	if (Member != nullptr)
	{
		Members.Remove(Member);
	}
}
