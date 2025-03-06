// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PJCheatManager.h"

#include "ConsoleSettings.h"

void UPJCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

	UConsoleSettings* ConsoleSettings = GetMutableDefault<UConsoleSettings>();

	if (ConsoleSettings != nullptr)
	{
		ConsoleSettings->ManualAutoCompleteList.Add(FAutoCompleteCommand(TEXT("PJEnemyNoDamage")));
	}
}

void UPJCheatManager::PJEnemyNoDamage(bool Flag)
{
	bNoDamage = Flag;
}
