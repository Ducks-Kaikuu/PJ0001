// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/PJEnemyManager.h"

void UPJEnemyManager::AddEnemy(APJEnemy* Enemy)
{
	EnemyList.Add(Enemy);
}

void UPJEnemyManager::RemoveEnemy(APJEnemy* Enemy)
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
