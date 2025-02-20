// Fill out your copyright notice in the Description page of Project Settings.


#include "PJ0000/Character/Action/PJAttackAction.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PJ0000/Character/Components/SNComboComponent.h"

bool UPJAttackAction::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	ASNCharacterBase* Character(GetOwner<ASNCharacterBase>());

	if (Character != nullptr)
	{
		USNComboComponent* ComboComponent = Character->FindComponentByClass<USNComboComponent>();

		if (ComboComponent != nullptr)
		{
			if (ComboComponent->IsComponentTickEnabled() == false)
			{
				ComboComponent->SetComboScore(0.0f);

				ComboComponent->Start();
				
				ComboComponent->SetComponentTickEnabled(true);
				
				SNPLUGIN_LOG(TEXT("Combo Start Comming. %f"), ComboComponent->GetComboScore());
				
			} else
			{
				if (Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(AdvancedInput))
				{
					if (ComboComponent->IsAdvancedInput() == false)
					{
						float ComboScore = ComboComponent->GetComboScore();
						
						ComboScore += 1.0f;

						if ((ComboScore == 4.0f) && (ComboComponent->IsLanded() == false))
						{
							FVector Start(Character->GetActorLocation());
							FVector End(Start);

							End.Z -= 10000.0f;

							TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes
							{
								UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
								UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)
								//UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)
							};

							TArray<FHitResult> Hits;
							
							UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), Start, End, 100.0f, 200.0f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hits, true);

							for (auto& Hit : Hits)
							{
								UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Hit.GetComponent());

								if (StaticMeshComponent != nullptr)
								{
									UMotionWarpingComponent* MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();

									if (MotionWarpingComponent != nullptr)
									{
										FTransform Transform = FTransform::Identity;

										Transform.SetTranslation(Hit.ImpactPoint);

										MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(TEXT("AttackLand"), Transform);
										
										break;
									}
								}
							}
						}

						ComboComponent->SetComboScore(ComboScore);

						SNPLUGIN_LOG(TEXT("Advanced Input Coming. %f"), ComboScore);
					}
					
					ComboComponent->SetAdvancedInput(true);

					Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(AdvancedInput);

					return true;
				}
			}
		}
	}

	return false;
}
