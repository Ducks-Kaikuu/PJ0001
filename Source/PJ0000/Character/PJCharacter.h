// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PJFighterInterface.h"
#include "MotionMatching/Character/SNMotionMatchingPlayerBase.h"
#include "Utility/SNAtomicFlag.h"
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

UENUM(BlueprintType)
enum class EPlayerStatusFlag : uint8
{
	NoDamage=0 UMETA(DisplayName = "No Damage"),
};

/**
 * 
 */
UCLASS()
class PJ0000_API APJCharacter : public ASNMotionMatchingPlayerBase, public IPJFighterInterface
{
	GENERATED_BODY()

public:
	APJCharacter(const FObjectInitializer& ObjectInitializer);

	bool IsLanded() const ;

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

	void SetNoDamage(bool bNoDamage);

	void SetMoveInput(const FVector2D& Value);

	const FVector2D& GetMoveInput() const;
	
	bool IsNoDamage() const;


	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	virtual bool IsDead() const;
protected:
	virtual void BeginPlay() override;
	
private:

	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	FVector2D MoveInput = FVector2D::ZeroVector;
	
	SNAtomicFlag<1> StatusFlag;
};

FORCEINLINE void APJCharacter::SetNoDamage(bool bNoDamage)
{
	if (bNoDamage == true)
	{
		StatusFlag.On(static_cast<int>(EPlayerStatusFlag::NoDamage));
	} else
	{
		StatusFlag.Off(static_cast<int>(EPlayerStatusFlag::NoDamage));
	}
}

FORCEINLINE bool APJCharacter::IsNoDamage() const
{
	return StatusFlag.Is(static_cast<int>(EPlayerStatusFlag::NoDamage));
}

FORCEINLINE void APJCharacter::SetMoveInput(const FVector2D& Value)
{
	MoveInput = Value;
}

FORCEINLINE const FVector2D& APJCharacter::GetMoveInput() const
{
	return MoveInput;
}
