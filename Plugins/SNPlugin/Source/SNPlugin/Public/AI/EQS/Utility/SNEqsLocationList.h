// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SNEqsLocationList.generated.h"

/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNEqsLocationList : public UObject
{
	GENERATED_BODY()

public:

	void AddLocation(float Score, const FVector& Location);

	const TArray<FVector4>& GetLocations() const ;

private:
	
	UPROPERTY()
	TArray<FVector4> Locations;
};

FORCEINLINE const TArray<FVector4>& USNEqsLocationList::GetLocations() const
{
	return Locations;
}
