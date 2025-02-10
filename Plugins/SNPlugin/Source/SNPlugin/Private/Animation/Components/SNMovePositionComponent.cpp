// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Components/SNMovePositionComponent.h"

#include "Online/Auth.h"

// Sets default values for this component's properties
USNMovePositionComponent::USNMovePositionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}


// Called when the game starts
void USNMovePositionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
	// ...
	
}


// Called every frame
void USNMovePositionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USNMovePositionComponent::AddTransform(const FName& Name, const FTransform& Transform, float StartTime)
{
	if (TransformMap.Contains(Name))
	{
		TransformMap[Name].StartTransform = Transform;

		TransformMap[Name].StartTime = StartTime;
	} else
	{
		FTransformData TransformData;

		TransformData.StartTransform = Transform;
		
		TransformMap.Add(Name, TransformData);
	}
}

void USNMovePositionComponent::RemoveTransform(const FName& Name)
{
	if (TransformMap.Contains(Name))
	{
		TransformMap.Remove(Name);
	}
}

const FTransformData* USNMovePositionComponent::GetTransformData(const FName& Name) const
{
	if (TransformMap.Contains(Name))
	{
		return &TransformMap[Name];
	}

	return nullptr;
}

