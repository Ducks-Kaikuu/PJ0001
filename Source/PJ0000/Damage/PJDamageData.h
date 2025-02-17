// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PJDamageData.generated.h"

USTRUCT(BlueprintType)
struct FDamageTable:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;
	
	UPROPERTY(EditAnywhere)
	float Damage = 1.0f;
	
	UPROPERTY(EditAnywhere)
	float Duration = 1.0f;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DamageCueTags;
};

USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Damage = 1.0f;
};


UCLASS(Blueprintable)
class UDamageData : public UDataAsset
{
	GENERATED_BODY()

public:

	void Initialize();

	TArray<const FDamageTable*> GetDamageList(const FGameplayTagContainer& DamageTags);
	
	const FDamageTable* GetDamage(const FGameplayTag& Tag) const; 

private:
	
	UPROPERTY(EditAnywhere, Category = "Damage")
	TMap<FGameplayTag, FDamage> DamageTables;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSoftObjectPtr<UDataTable> DamageDataTable= nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> DamageData = nullptr;
};
