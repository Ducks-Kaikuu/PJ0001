// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNAttackComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
USNAttackComponent::USNAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}

FHitResult USNAttackComponent::GenerateSphereSweep(AActor* pAttacker, float Time, const FVector& Start,
	const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList,
	bool bPenetrate, bool bBomb)
{
	return InternalGenerateSphereSweep(pAttacker, Time, Start, End, Radius, GameplayTags, ignoreList, bPenetrate, bBomb);
}

FHitResult USNAttackComponent::GenerateSphere(AActor* pAttacker, float Time, const FVector& Pos, float Radius,
	const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList)
{
	return InternalGenerateSphereSweep(pAttacker, Time, Pos, Pos, Radius, GameplayTags, ignoreList, true, true);
}


// Called when the game starts
void USNAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	// Tickは回さない
	SetComponentTickEnabled(false);
	// ...
	
}

FHitResult USNAttackComponent::InternalGenerateSphereSweep(AActor* pAttacker, float Time, const FVector& Start, const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, bool bPenetrate, bool bBomb)
{
	if (pAttacker == nullptr)
	{
		return FHitResult();
	}

	if (pAttacker->HasActorBegunPlay() == false)
	{
		return FHitResult();
	}
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return FHitResult();
	}
	
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;

#if WITH_EDITORONLY_DATA
	if (bDebugDraw == true)
	{
		DebugType = EDrawDebugTrace::ForDuration;
	}
#endif
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes
	{
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)
		//UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)
	};

	TArray<FHitResult> Results;
	
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(World,
		Start, End, Radius,
		ObjectTypes, true,
		ignoreList, DebugType, Results, true);

	for(auto Result:Results){
		
		if(Result.bBlockingHit == true){
			// 攻撃判定チェック
			eHitState state = OnHit(Result, GameplayTags, Time, pAttacker);
			
			if(bPenetrate == false){
				return Result;
			} else {
				// 貫通属性の場合、背景などにあたった場合は判定を終了する
				// ただし、爆発などの場合は終了しない
				if((bBomb == false) && (state == eHitState_HitNonAttackObject)){
					return Result;
				}
			}
		}
	}

	return FHitResult();

}

USNAttackComponent::eHitState USNAttackComponent::OnHit(const FHitResult& Result, const FGameplayTagContainer& GameplayTags, float Time, AActor* pAttacker)
{
	AActor* Target = Result.GetActor();

	if (Target != nullptr)
	{
		UAbilitySystemComponent* AbilitySystemComponent = Target->FindComponentByClass<UAbilitySystemComponent>();

		if (AbilitySystemComponent != nullptr)
		{
			USNDamageWithChooserComponent* DamageComponent = Target->FindComponentByClass<USNDamageWithChooserComponent>();
			
			if (DamageComponent != nullptr)
			{
				FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

				FGameplayEffectContext* Context = ContextHandle.Get();

				if (Context != nullptr)
				{
					TArray<TWeakObjectPtr<AActor>> ActorList = {pAttacker};

					Context->AddActors(ActorList);
					
					Context->AddHitResult(Result);
				}
				// EffectContextをDamageComponentに持たせることに何か不満...
				DamageComponent->SetDamagedEffectContextHandle(ContextHandle);
			}
			
			AbilitySystemComponent->TryActivateAbilitiesByTag(GameplayTags);
#if 0
			{
				for (auto& GameplayTag : GameplayTags)
				{
					AbilitySystemComponent->ExecuteGameplayCue(GameplayTag, ContextHandle);	
				}
			};
#endif
			return eHitState_HitAttackCanDamage;
		}

		return eHitState_HitAttackCannotDamage;
	}
	// 攻撃できないオブジェクトにあたった
	return eHitState_HitNonAttackObject;
}


