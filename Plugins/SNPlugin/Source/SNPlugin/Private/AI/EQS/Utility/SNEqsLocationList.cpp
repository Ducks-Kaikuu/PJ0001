// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/Utility/SNEqsLocationList.h"

void USNEqsLocationList::AddLocation(float Score, const FVector& Location)
{
	FVector4 NewLocation = Location;

	NewLocation.W = Score;

	Locations.Add(NewLocation);
}

void USNEqsLocationList::SortLocations()
{
	Locations.Sort([](const FVector4& A, const FVector4& B)
	{
		return A.W > B.W;
	});
}

void USNEqsLocationList::ClearLocations()
{
	Locations.Empty();
}

void USNEqsLocationList::RemoveAt(int32 Index)
{
	if (Locations.Num() > Index)
	{
		Locations.RemoveAt(Index);
	}
}

TArray<FVector> USNEqsLocationList::GetHighScoreLocations() const
{
	if (Locations.IsEmpty() == true){
		return TArray<FVector>();
	}

	return GetLocationsHigherThanScore(Locations[0].W);
}

TArray<FVector> USNEqsLocationList::GetLocationsHigherThanScore(float Score) const
{
	if (Locations.IsEmpty() == true){
		return TArray<FVector>();
	}
		
	TArray<FVector> Result;

	for (auto& Location : Locations)
	{
		if (Location.W >= Score)
		{
			Result.Add(Location);
		} else
		{
			break;
		}
	}

	return Result;
}

