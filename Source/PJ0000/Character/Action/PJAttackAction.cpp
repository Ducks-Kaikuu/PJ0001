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

bool UPJAttackAction::ExecAction(const FInputActionValue& InputActionValue){
	
	Super::ExecAction(InputActionValue);
	
	bool Result = false;
	
	ASNCharacterBase* Character(GetOwner<ASNCharacterBase>());
	
	if(Character != nullptr){
		
		USNComboComponent* ComboComponent = Character->FindComponentByClass<USNComboComponent>();
		
		if(ComboComponent != nullptr){
			// Tickが動いてない場合は初回攻撃とする
			if(ComboComponent->IsComponentTickEnabled() == false){
				// コンボ数を0クリア
				ComboComponent->SetComboScore(0.0f);
				// コンボスタート
				ComboComponent->Start();
				// Tickを有効化
				ComboComponent->SetComponentTickEnabled(true);
				
				SNPLUGIN_LOG(TEXT("Combo Start Comming. %f"), ComboComponent->GetComboScore());
				
				Result = true;
				
			} else {
				// 先行入力のリクエストがされているかチェック
				if(Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(AdvancedInput)){
					
					if(ComboComponent->IsAdvancedInput() == false){
						
						float ComboScore = ComboComponent->GetComboScore();
						
						ComboScore += 1.0f;
						// 空中でのコンボの5段目(地面への降下攻撃)かチェック
						if((ComboScore == 4.0f) && (ComboComponent->IsLanded() == false)){
							
							FVector Start(Character->GetActorLocation());
							FVector End(Start);
							
							End.Z -= 10000.0f;
							
							TArray<FHitResult> Hits;
							// 地面の判定を取りたい
							UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), Start, End, 100.0f, 200.0f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hits, true);
							
							for(auto& Hit : Hits){
								// 背景メッシュかチェック
								UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Hit.GetComponent());
								
								if(StaticMeshComponent != nullptr){
									
									UMotionWarpingComponent* MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
									
									if(MotionWarpingComponent != nullptr){
										
										FTransform Transform = FTransform::Identity;
										
										Transform.SetTranslation(Hit.ImpactPoint);
										
										MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(TEXT("AttackLand"), Transform);
										
										break;
									}
								}
							}
						}
						// コンボ数を設定
						ComboComponent->SetComboScore(ComboScore);
						
						SNPLUGIN_LOG(TEXT("Advanced Input Coming. %f"), ComboScore);
					}
					// 先行入力フラグをON
					ComboComponent->SetAdvancedInput(true);
					
					Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(AdvancedInput);
					
					Result = true;
				}
			}
		}
	}
	
	return Result;
}
