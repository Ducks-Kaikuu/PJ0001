// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SNAbilitySystemComponent.generated.h"

class USNAttributeSet;
struct FStreamableHandle;
class USNAttribute;
/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	USNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void AddGameplayTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveGameplayTags(const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveAllGameplayTags();

	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void AddAbility(UGameplayAbility* AbilityTask);

	UFUNCTION(BlueprintCallable, Category = "SN|AbilitySystem")
	void RemoveAbility(UGameplayAbility* AbilityTask);

	void SetAttribute(TSubclassOf<USNAttributeSet> Attribute);
protected:
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void LoadedAbility();

	TSharedPtr<FStreamableHandle> StreamHandle;

	UPROPERTY(EditAnywhere, Category = "SN|AbilitySystem")
	TArray<TSoftClassPtr<UGameplayAbility>> AbilityTasks;
	
	UPROPERTY()
	TMap<TObjectPtr<UClass>, FGameplayAbilitySpecHandle> AbilityTaskList;
};

