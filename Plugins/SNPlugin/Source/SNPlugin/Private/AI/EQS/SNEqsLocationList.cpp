// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/Utility/SNEqsLocationList.h"

void USNEqsLocationList::AddLocation(float Score, const FVector& Location)
{
	FVector4 NewLocation = Location;

	NewLocation.W = Score;

	Locations.Add(NewLocation);

	Locations.Sort([](const FVector4& A, const FVector4& B)
	{
		return A.W > B.W;
	});
}

