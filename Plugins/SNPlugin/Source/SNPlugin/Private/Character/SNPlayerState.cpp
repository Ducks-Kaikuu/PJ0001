// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SNPlayerState.h"

ASNPlayerState::ASNPlayerState(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	SetNetUpdateFrequency(100.0f);
}
