// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PJFighterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UPJFighterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PJ0000_API IPJFighterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual bool IsDead() const;
};
