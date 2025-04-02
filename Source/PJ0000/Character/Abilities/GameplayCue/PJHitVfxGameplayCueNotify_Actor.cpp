// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/GameplayCue/PJHitVfxGameplayCueNotify_Actor.h"

#include "SNDef.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/PJDamageWithChooserComponent.h"
#include "System/PJGameInstance.h"
#include "System/PJTimerManager.h"
#include "Utility/SNUtility.h"

APJHitVfxGameplayCueNotify_Actor::APJHitVfxGameplayCueNotify_Actor()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag((TEXT("GameplayCue.HitVfx")));
}

void APJHitVfxGameplayCueNotify_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	UPJDamageWithChooserComponent* DamageComponent(MyTarget->FindComponentByClass<UPJDamageWithChooserComponent>());

	if (DamageComponent != nullptr)
	{
		FGameplayEffectContextHandle ContextHandle(DamageComponent->GetDamagedEffectContextHandle());

		if (ContextHandle.IsValid())
		{
			const FHitResult* HitResult(ContextHandle.GetHitResult());

			if (HitResult != nullptr)
			{
				UPJTimerManager* TimerManager = nullptr;
				
				UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

				if (GameInstance != nullptr)
				{
					TimerManager = GameInstance->GetTimerManager();	
				}

				UNiagaraComponent* Component = nullptr;
				
				ASNCharacterBase* Character = Cast<ASNCharacterBase>(MyTarget);

				if (Character != nullptr)
				{
					bool bPlayer = false;
					
					for (auto& Category:Character->GetCategoryType())
					{
						if (Category.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Category.Player"))))
						{
							bPlayer = true;

							break;
						}
					}

					if (bPlayer == true)
					{
						Component = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PlayerHitVfx, HitResult->ImpactPoint, FRotator::ZeroRotator, Scale);
					} else
					{
						Component = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnemyHitVfx, HitResult->ImpactPoint, FRotator::ZeroRotator, Scale);
					}
				} else
				{
					Component = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVfx, HitResult->ImpactPoint, FRotator::ZeroRotator, Scale);
				}

				if ((TimerManager != nullptr) && (Component != nullptr))
				{
					TimerManager->AddIgnoreTimeRareVfx(Component);
				}
			}
		}
	}
	
	
	SNPLUGIN_LOG(TEXT("Hit Vfx Handler Called Event id = %d"), EventType);
}
