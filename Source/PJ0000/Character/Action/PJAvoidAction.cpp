// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Action/PJAvoidAction.h"

#include "RayTracingBuiltInResources.h"
#include "Character/PJCharacter.h"
#include "Character/Base/SNCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/SNBlueprintFunctionLibrary.h"

bool UPJAvoidAction::ExecAction(const FInputActionValue& InputActionValue)
{
	bool Result = Super::ExecAction(InputActionValue);

	APJCharacter* Character = GetOwner<APJCharacter>();

	if (Character == nullptr)
	{
		return false;
	}

	if (Character->IsLanded() == false)
	{
		return false;
	}

	FVector ForwardVector(Character->GetActorForwardVector());

	FVector2D InputValue = Character->GetMoveInput();

	FVector Velocity(InputValue.Y, InputValue.X, 0.0f);

	Velocity = Character->GetActorRotation().RotateVector(Velocity);

	Velocity = Velocity.GetSafeNormal();

	Character->SetMoveInput(FVector2D::ZeroVector);
	//FVector Velocity(Character->GetVelocity().GetSafeNormal());

	if (Velocity.IsNearlyZero())
	{
		AxisX = 0.0f;
	} else
	{
		AxisX = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, Velocity)));

		FVector Cross = FVector::CrossProduct(ForwardVector, Velocity);

		if (Cross.Z < 0.0f)
		{
			AxisX =-AxisX;
		}	
	}

	float PlayRate = 1.0f;
	float StartTime = 0.0f;

	UAnimMontage* DodgeAnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(Character, DodgeChooserTable, this, PlayRate, StartTime);

	UKismetSystemLibrary::PrintString(GetWorld(), FString(TEXT("Dodge:")) + FString::SanitizeFloat(AxisX));

	UAnimInstance* AnimInstance = Character->GetAnimInstance();

	if (AnimInstance == nullptr)
	{
		return false;
	}

	UAnimMontage* AnimMontage = AnimInstance->GetCurrentActiveMontage();

	if (AnimMontage != nullptr)
	{
		AnimInstance->Montage_Stop(0.25f, AnimMontage);
	}

	AnimInstance->Montage_Play(DodgeAnimMontage);
	
	return Result;
}
