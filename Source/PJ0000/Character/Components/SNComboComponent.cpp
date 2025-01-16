// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Components/SNComboComponent.h"

#include "ChooserFunctionLibrary.h"
#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "System/SNBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
USNComboComponent::USNComboComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void USNComboComponent::Start()
{
	MontageProxyInstance = PerformMotionMatching(Cast<ACharacter>(GetOwner()));

	if (MontageProxyInstance != nullptr)
	{
		MontageProxyInstance->OnCompleted.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
		MontageProxyInstance->OnInterrupted.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
		MontageProxyInstance->OnBlendOut.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
	}
}

void USNComboComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ASNCharacterBase* CharacterBase = Cast<ASNCharacterBase>(GetOwner());

	USNAbilitySystemComponent* AbilitySystemComponent = CharacterBase->GetAbilitySystemComponent();
	
	if (AbilitySystemComponent != nullptr){
		
		if (AbilitySystemComponent->HasMatchingGameplayTag(BranchTag) == true)
		{
			if (bAdvancedInput == true)
			{
				bAdvancedInput = false;

				if (MontageProxyInstance != nullptr)
				{
					MontageProxyInstance->OnCompleted.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
					MontageProxyInstance->OnInterrupted.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
					MontageProxyInstance->OnBlendOut.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
				}
				
				MontageProxyInstance = PerformMotionMatching(Cast<ACharacter>(GetOwner()));

				AbilitySystemComponent->RemoveLooseGameplayTag(BranchTag);
				
				if (MontageProxyInstance != nullptr)
				{
					MontageProxyInstance->OnCompleted.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
					MontageProxyInstance->OnInterrupted.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
					MontageProxyInstance->OnBlendOut.AddDynamic(this, &USNComboComponent::OnMontagePlayEnd);
				} else
				{
					FinishCombo();	
				}
			}
		}
	}
}

void USNComboComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

void USNComboComponent::FinishCombo()
{
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(GetOwner()));

	if (Character != nullptr)
	{
		Character->RemoveActionTag(FGameplayTag::RequestGameplayTag(TEXT("Input.Attack.Combo"), true));	
	}
	
	ComboScore = 0.0f;
	
	SetComponentTickEnabled(false);

	SNPLUGIN_LOG(TEXT("USNComboComponent::FinishCombo"));
}

void USNComboComponent::OnMontagePlayEnd(FName NotiryName)
{
	FinishCombo();

	if (MontageProxyInstance != nullptr)
	{
		MontageProxyInstance->OnCompleted.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
		MontageProxyInstance->OnInterrupted.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
		MontageProxyInstance->OnBlendOut.RemoveDynamic(this, &USNComboComponent::OnMontagePlayEnd);
	}
}

bool USNComboComponent::IsLanded() const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character != nullptr)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

		if (MovementComponent != nullptr)
		{
			if ((MovementComponent->MovementMode == MOVE_Flying)
			|| (MovementComponent->MovementMode == MOVE_Falling))
			{
				return false;
			}
		}
	}
	
	return true;
}

UPlayMontageCallbackProxy* USNComboComponent::PerformMotionMatching(ACharacter* Character)
{
	if (AttackAnimationChooser != nullptr)
	{
		float PlayRate = 1.0f;
		float StartTime = 0.0f;
		
		UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary::GetAnimMontageFromChooser(Character, AttackAnimationChooser, this, PlayRate, StartTime);
	
		const UAnimMontage* ClimbMontage = Cast<const UAnimMontage>(AnimMontage);
		// モンタージュを再生
		UPlayMontageCallbackProxy* MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(Character->GetMesh(), const_cast<UAnimMontage*>(ClimbMontage), PlayRate, StartTime, NAME_None);

		return MontageProxy;
	}

	return nullptr;
}

void USNComboComponent::AddAbilityTask(UAbilityTask* Task)
{
	if (Task != nullptr)
	{
		ComboTasks.Add(Task);	
	}

	if (ComboTasks.Num() > 0)
	{
		SetComponentTickEnabled(true);
	}
}

void USNComboComponent::RemoveAbilityTask(UAbilityTask* Task)
{
	ComboTasks.Remove(Task);
}



