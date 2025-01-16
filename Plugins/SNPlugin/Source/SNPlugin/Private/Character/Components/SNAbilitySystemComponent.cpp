// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNAbilitySystemComponent.h"

#include "SNDef.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Utility/SNUtility.h"

USNAbilitySystemComponent::USNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USNAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	TArray<FSoftObjectPath> assetList;

	for (auto& Path:AbilityTasks)
	{
		assetList.Add(Path.ToSoftObjectPath());
	}

	// ロードをリクエスト
	StreamHandle = SNUtility::RequestAsyncLoad<USNAbilitySystemComponent>(assetList, this, &USNAbilitySystemComponent::LoadedAbility);
}

void USNAbilitySystemComponent::LoadedAbility()
{
	if (StreamHandle.IsValid())
	{
		TArray<UObject*> Assets;
		
		StreamHandle->GetLoadedAssets(Assets);

		for (auto& Asset : Assets)
		{
			UClass* Class = Cast<UClass>(Asset);

			if (Class != nullptr)
			{
				UGameplayAbility* Task = NewObject<UGameplayAbility>(GetOwner(), Class, TEXT("AbilityTask"));

				if (Task != nullptr)
				{
					AddAbility(Task);
				}
			}
		}
	}
}

void USNAbilitySystemComponent::AddGameplayTags(const FGameplayTagContainer& Tags)
{
	AddLooseGameplayTags(Tags);
}

void USNAbilitySystemComponent::RemoveGameplayTags(const FGameplayTagContainer& Tags)
{
	RemoveLooseGameplayTags(Tags);
}

void USNAbilitySystemComponent::RemoveAllGameplayTags()
{
	GameplayTagCountContainer.Reset();
}

void USNAbilitySystemComponent::AddAbility(UGameplayAbility* AbilityTask)
{
	if (AbilityTask != nullptr)
	{
		FGameplayAbilitySpecHandle Handle = GiveAbility(FGameplayAbilitySpec(AbilityTask));

		AbilityTaskList.Add(AbilityTask, Handle);
	}
}

void USNAbilitySystemComponent::RemoveAbility(UGameplayAbility* AbilityTask)
{
	if (AbilityTaskList.Contains(AbilityTask) == true)
	{
		FGameplayAbilitySpecHandle Handle(AbilityTaskList[AbilityTask]);

		SetRemoveAbilityOnEnd(Handle);

		AbilityTaskList.Remove(AbilityTask);
	} 
}
