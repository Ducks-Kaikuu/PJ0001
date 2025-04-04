// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNEqsLocationList.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SNPLUGIN_API USNEqsLocationList : public UObject
{
	GENERATED_BODY()

public:

	void AddLocation(float Score, const FVector& Location);

	void SortLocations();

	const TArray<FVector4>& GetLocations() const ;

	void ClearLocations();

	FVector GetHighScoreLocation() const ;

	TArray<FVector> GetHighScoreLocations() const ;

	TArray<FVector> GetLocationsHigherThanScore(float Score) const ;
private:
	// EQSで生成されたクエリをロケーションとして保持。
	// W値にスコアが入っていて、スコアが高い順にソートされています。
	UPROPERTY()
	TArray<FVector4> Locations;
};

FORCEINLINE const TArray<FVector4>& USNEqsLocationList::GetLocations() const
{
	return Locations;
}

FORCEINLINE FVector USNEqsLocationList::GetHighScoreLocation() const
{
	return Locations.IsEmpty() == true ? FVector::ZeroVector : FVector(Locations[0]);
} 