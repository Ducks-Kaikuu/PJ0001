// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "PJCharacter.generated.h"

class UPJHealthSet;
class UGameplayAbility;
class AActor;
struct FGameplayEffectSpec;

USTRUCT(BlueprintType)
struct FSimpleCameraParameter{

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpringArmLength = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SocketOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TranslationLagSpeed = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FieldOfView = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TransitionSpeed = 0.0f;
};

/**
 * 
 */
UCLASS()
class PJ0000_API APJCharacter : public ASNMotionMatchingPlayerBase
{
	GENERATED_BODY()

public:
	APJCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void UpdateCamera();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Camera)
	FSimpleCameraParameter FarCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Camera)
	FSimpleCameraParameter NearCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Camera)
	FSimpleCameraParameter AimCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Camera)
	float CameraDistanceMultiplier = 1.0f;

protected:
	virtual void BeginPlay() override;
	
private:

	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
};
