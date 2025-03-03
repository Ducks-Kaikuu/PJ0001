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

void USNDamageWithChooserComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (MontageProxy!= nullptr)
	{
		MontageProxy->OnCompleted.RemoveAll(this);
		
		MontageProxy = nullptr;
	}
}

void USNDamageWithChooserComponent::OnMontagePlayEnd(FName NotifyName)
{
	if (OnMontagePlayEndDelegate.IsBound())
	{
		OnMontagePlayEndDelegate.Broadcast();
	}
	
	SNPLUGIN_LOG(TEXT("USNDamageWithChooserComponent::OnMontagePlayEnd."));
}

void USNDamageWithChooserComponent::OnMontageInterrupted(FName NotifyName)
{
	if (OnMontageInterruptedDelegate.IsBound())
	{
		OnMontageInterruptedDelegate.Broadcast();
	}
	SNPLUGIN_LOG(TEXT("USNDamageWithChooserComponent::OnMontageInterrupted."));
}

void USNDamageWithChooserComponent::OnMontageBlendOut(FName NotifyName)
{
	if (OnMontageBlendOutDelegate.IsBound())
	{
		OnMontageBlendOutDelegate.Broadcast();
	}
	SNPLUGIN_LOG(TEXT("USNDamageWithChooserComponent::OnMontageBlendOut."));
}

void USNDamageWithChooserComponent::OnNotifyBegin(FName NotifyName)
{
	if (OnNotifyBeginDelegate.IsBound())
	{
		OnNotifyBeginDelegate.Broadcast(NotifyName);
	}
	
	SNPLUGIN_LOG(TEXT("USNDamageWithChooserComponent::OnNotifyBegin."));
}

void USNDamageWithChooserComponent::OnNotifyEnd(FName NotifyName)
{
	if (OnNotifyEndDelegate.IsBound())
	{
		OnNotifyEndDelegate.Broadcast(NotifyName);
	}
	
	SNPLUGIN_LOG(TEXT("USNDamageWithChooserComponent::OnNotifyEnd."));
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
				MontageProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(Character->GetMesh(), AnimMontage, 1.0f, 0.0f);

				if (MontageProxy!= nullptr)
				{
					if (bAddToOwner == true)
					{
						Character->AddActionTagContainer(DamageTags);
					}

					if (MontageProxy->OnCompleted.Contains(this, TEXT("USNDamageWithChooserComponent::OnMontagePlayEnd")) == false)
					{
						MontageProxy->OnCompleted.AddDynamic(this, &USNDamageWithChooserComponent::OnMontagePlayEnd);
					}

					if (MontageProxy->OnInterrupted.Contains(this, TEXT("USNDamageWithChooserComponent::OnMontageInterrupted")) == false)
					{
						MontageProxy->OnInterrupted.AddDynamic(this, &USNDamageWithChooserComponent::OnMontageInterrupted);
					}

					if (MontageProxy->OnBlendOut.Contains(this, TEXT("USNDamageWithChooserComponent::OnMontageBlendOut")) == false)
					{
						MontageProxy->OnBlendOut.AddDynamic(this, &USNDamageWithChooserComponent::OnMontageBlendOut);
					}

					if (MontageProxy->OnNotifyBegin.Contains(this, TEXT("USNDamageWithChooserComponent::OnNotifyBegin")) == false)
					{
						MontageProxy->OnNotifyBegin.AddDynamic(this, &USNDamageWithChooserComponent::OnNotifyBegin);
					}

					if (MontageProxy->OnNotifyEnd.Contains(this, TEXT("USNDamageWithChooserComponent::OnNotifyEnd")) == false)
					{
						MontageProxy->OnNotifyEnd.AddDynamic(this, &USNDamageWithChooserComponent::OnNotifyEnd);
					}
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

