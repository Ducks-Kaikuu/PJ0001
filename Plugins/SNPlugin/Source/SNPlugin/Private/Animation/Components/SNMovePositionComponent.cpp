// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Components/SNMovePositionComponent.h"

// Sets default values for this component's properties
USNMovePositionComponent::USNMovePositionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USNMovePositionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USNMovePositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USNMovePositionComponent::AddTargetTransform(const FName& Name, const FTransform& Transform)
{
	if (TransformMap.Contains(Name))
	{
		TransformMap[Name].DestTransform = Transform;
	} else
	{
		FTransformData TransformData;

		TransformData.DestTransform = Transform;
		
		TransformMap.Add(Name, TransformData);
	}
}

void USNMovePositionComponent::AddSourceTransform(const FName& Name, const FTransform& Transform)
{
	if (TransformMap.Contains(Name))
	{
		TransformMap[Name].SrcTransform = Transform;
	} else
	{
		FTransformData TransformData;

		TransformData.SrcTransform = Transform;
		
		TransformMap.Add(Name, TransformData);
	}
}

void USNMovePositionComponent::SetStateTotalDuration(const FName& Name, float Duration)
{
	if (TransformMap.Contains(Name)){
		TransformMap[Name].TotalDuration = Duration;
	}
}

float USNMovePositionComponent::GetStateTotalDuration(const FName& Name) const
{
	if (TransformMap.Contains(Name))
	{
		return TransformMap[Name].TotalDuration;
	}
	
	return 0.0f;
}

const FTransform& USNMovePositionComponent::GetDestTransform(const FName& Name) const
{
	if (TransformMap.Contains(Name))
	{
		return TransformMap[Name].DestTransform;
	}

	return FTransform::Identity;
}

const FTransform& USNMovePositionComponent::GetSrcTransform(const FName& Name) const
{
	if (TransformMap.Contains(Name))
	{
		return TransformMap[Name].SrcTransform;
	}

	return FTransform::Identity;
}

