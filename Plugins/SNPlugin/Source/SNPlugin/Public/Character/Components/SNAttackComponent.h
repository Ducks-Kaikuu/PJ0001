// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNAttackComponent.generated.h"


struct FGameplayTagContainer;

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class SNPLUGIN_API USNAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNAttackComponent();

	//! @{@name 球型のスイープコリジョンを生成
	FHitResult	GenerateSphereSweep(AActor* pAttacker, float Time, const FVector& Start, const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, bool isPenetrate, bool bBomb);
	//! @}

	//! @{@name 球型の攻撃コリジョンを生成
	FHitResult	GenerateSphere(AActor* pAttacker, float Time, const FVector& Pos, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList);
	//! @}
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//!< @brief OnHitの返り値
	enum eHitState{
		eHitState_HitAttackCanDamage,
		eHitState_HitAttackCannotDamage,
		eHitState_HitNonAttackObject,
	};
	
	//! @{@name 球型のスイープコリジョンを生成
	FHitResult	InternalGenerateSphereSweep(AActor* pAttacker, float Time, const FVector& Start, const FVector& End, float Radius, const FGameplayTagContainer& GameplayTags, const TArray<AActor*>& ignoreList, bool bPenetrate, bool bBomb);
	//! @}

	//! @{@name 攻撃コリジョンが当たった際の処理
	eHitState	OnHit(const FHitResult& Result, const FGameplayTagContainer& GameplayTags, float Time, AActor* pAttacker);
	//! @}
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, DisplayName="DrawCollision", Category = "SN|Attack|Debug")
	bool bDebugDraw = false;
#endif
	
};
