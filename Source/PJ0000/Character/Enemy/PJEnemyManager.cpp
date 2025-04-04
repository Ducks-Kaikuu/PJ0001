// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/PJEnemyManager.h"

void UPJEnemyManager::AddEnemy(APJEnemyBase* Enemy)
{
	EnemyList.Add(Enemy);
}

void UPJEnemyManager::RemoveEnemy(APJEnemyBase* Enemy)
{
	if (EnemyList.Contains(Enemy))
	{
		EnemyList.Remove(Enemy);	

		if (EnemyList.Num() <= 0)
		{
			if (OnEnemyGone.IsBound())
			{
				OnEnemyGone.Broadcast();
			}
		}
	}
}
