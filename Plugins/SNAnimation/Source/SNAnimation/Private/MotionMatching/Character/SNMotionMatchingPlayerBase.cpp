// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "MotionWarpingComponent.h"

#include "MotionMatching/Traversal/Component/SNTraversalComponent.h"

ASNMotionMatchingPlayerBase::ASNMotionMatchingPlayerBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
	TraversalComponent = CreateDefaultSubobject<USNTraversalComponent>(TEXT("TraversalComponent"));
}
