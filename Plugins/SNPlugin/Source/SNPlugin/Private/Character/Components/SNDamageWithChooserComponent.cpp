// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNDamageWithChooserComponent.h"

#include "PlayMontageCallbackProxy.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "System/SNBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
USNDamageWithChooserComponent::USNDamageWithChooserComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}


// Called when the game starts
void USNDamageWithChooserComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	SetComponentTickEnabled(false);
}

UPlayMontageCallbackProxy* USNDamageWithChooserComponent::PlayDamageAnimation(const FGameplayTagContainer& DamageTags, bool bAddToOwner)
{
	if (DamageAnimationChooser == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("Damage Animation Chooser is nullptr."));

		return nullptr;
	}

	ASNCharacterBase* Character = Cast<ASNCharacterBase>(GetOwner());

	if (Character == nullptr)
	{
		SNPLUGIN_ERROR(TEXT("Damage Character is nullptr."));

		return nullptr;
	}

	float PlayRate = 1.0f;
	float StartTime = 0.0f;
	
	UAnimMontage* AnimMontage = USNBlueprintFunctionLibrary	::GetAnimMontageFromChooser(Character, DamageAnimationChooser, Character, PlayRate, StartTime);

	if (AnimMontage != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			if (AnimInstance->Montage_IsPlaying(AnimMontage) == false)
			{
				UPlayMontageCallbackProxy* MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(Character->GetMesh(), AnimMontage, 1.0f, 0.0f);

				if ((MontageProxy!= nullptr) && (bAddToOwner == true))
				{
					Character->AddActionTagContainer(DamageTags);
				}
#if WITH_EDITORONLY_DATA
				if (bDebugDraw == true)
				{
					SNPLUGIN_LOG(TEXT("Motion is %s"), *AnimMontage->GetName());
				}
#endif
				
				return MontageProxy;
			}
		}
	}

	return nullptr;
}

