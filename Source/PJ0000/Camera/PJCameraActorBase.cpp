// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/PJCameraActorBase.h"

APJCameraActorBase::APJCameraActorBase(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}
