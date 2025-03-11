// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "PJPlayerCameraManager.generated.h"

class APJCameraActorBase;
/**
 * 
 */
UCLASS()
class PJ0000_API APJPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

protected:

	APJPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

private:

	UPROPERTY(EditAnywhere, Category="PJ|Camera")
	TMap<FName, TSoftClassPtr<APJCameraActorBase>> CameraClassMap;

	UPROPERTY()
	TMap<FName, TObjectPtr<APJCameraActorBase>> CameraInstanceMap;

	FName CurrentCamera;
};
