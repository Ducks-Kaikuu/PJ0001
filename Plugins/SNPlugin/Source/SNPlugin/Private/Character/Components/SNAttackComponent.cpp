// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNAttackComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Components/SNDamageWithChooserComponent.h"
#include "Kismet/KismetSystemLibrary.h"

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
USNAttackComponent::USNAttackComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}

FHitResult USNAttackComponent::GenerateSphereSweep(AActor* pAttacker, const FVector& Start, const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, bool bPenetrate, bool bBomb, FGuid guid){
	return InternalGenerateSphereSweep(pAttacker, Start, End, Radius, GameplayTags, ignoreList, bPenetrate, bBomb, guid);
}

FHitResult USNAttackComponent::GenerateSphere(AActor* pAttacker, const FVector& Pos, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, FGuid guid){
	return InternalGenerateSphereSweep(pAttacker, Pos, Pos, Radius, GameplayTags, ignoreList, true, true, guid);
}

// Called when the game starts
void USNAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	// Tickは回さない
	SetComponentTickEnabled(false);
	// ...
	
}

FHitResult USNAttackComponent::InternalGenerateSphereSweep(AActor* pAttacker, const FVector& Start, const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, bool bPenetrate, bool bBomb, FGuid guid)
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

	if (guid.IsValid() == false)
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
			eHitState state = OnHit(Result, GameplayTags, pAttacker, guid);
			
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

USNAttackComponent::eHitState USNAttackComponent::OnHit(const FHitResult& Result, const FGameplayTagContainer& GameplayTags, AActor* Attacker, FGuid guid)
{
	ASNCharacterBase* Target = Cast<ASNCharacterBase>(Result.GetActor());

	if (Target != nullptr)
	{
		ASNCharacterBase* AttackCharacter = Cast<ASNCharacterBase>(Attacker);

		if (AttackCharacter != nullptr)
		{
			// 同じタイプ同志にはダメージを与えないための処理。ここでこの処理でいいのか...。様子を見ながら要検討。
			if (AttackCharacter->GetCategoryType().HasAny(Target->GetCategoryType()))
			{
				return eHitState_HitAttackCannotDamage;
			}
		}
		UAbilitySystemComponent* AbilitySystemComponent = Target->FindComponentByClass<UAbilitySystemComponent>();

		if (AbilitySystemComponent != nullptr)
		{
			USNDamageComponent* DamageComponent = Target->FindComponentByClass<USNDamageComponent>();
			
			if (DamageComponent != nullptr)
			{

				if (DamageComponent->GetDamageGuid() == guid)
				{
					return eHitState_HitAttackCannotDamage;
				}
				// ダメージの固有IDを取得
				DamageComponent->SetDamageGuid(guid);
				
				FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();

				FGameplayEffectContext* Context = ContextHandle.Get();

				if (Context != nullptr)
				{
					TArray<TWeakObjectPtr<AActor>> ActorList = {Attacker};

					Context->AddActors(ActorList);
					
					Context->AddHitResult(Result);
				}
				// EffectContextをDamageComponentに持たせることに何か不満...
				DamageComponent->SetDamagedEffectContextHandle(ContextHandle);
			}

			for(auto& AttackTag : GameplayTags){
				AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackTag));	
			}
			
			
			return eHitState_HitAttackCanDamage;
		}

		return eHitState_HitAttackCannotDamage;
	}
	// 攻撃できないオブジェクトにあたった
	return eHitState_HitNonAttackObject;
}


