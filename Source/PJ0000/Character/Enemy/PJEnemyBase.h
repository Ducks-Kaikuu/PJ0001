// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Character/Base/SNCharacterBase.h"
#include "Character/Interface/PJFighterInterface.h"
#include "PJEnemyBase.generated.h"

class FOnMontagePlayDelegate;
class UChooserTable;
class UPlayMontageCallbackProxy;
class UPJEnemyGroup;
class APJAIEnemy000;
struct FGenericTeamId;

/**
 * 
 */
UCLASS()
class PJ0000_API APJEnemyBase : public ASNCharacterBase, public IPJFighterInterface
{
	GENERATED_BODY()

public:
	
	APJEnemyBase(const FObjectInitializer& ObjectInitializer);
	
	APJAIEnemy000* GetAIController();
	const APJAIEnemy000* GetAIController() const ;

	void SetGroupManager(UPJEnemyGroup* Manager);

	void SetTeamID(const FGenericTeamId& TeamID);

	FGenericTeamId GetTeamID() const ;

	void SetDistanceToPlayer(float Distance);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	bool IsDead() const;

	void ResetMontagePlayProxy(){ MontageProxy = nullptr; }	

	virtual void Landed(const FHitResult& Hit) override;

	DECLARE_DELEGATE_OneParam(FLandedDelegate, const FHitResult&);
	FLandedDelegate OnLanded;

	UPlayMontageCallbackProxy* PlayAnimMontageByActionTag();

	void SetAttackMotionDelegate(UPlayMontageCallbackProxy* Proxy, const FGameplayTag& Tag, FOnMontagePlayDelegate& Delegate);

	void SetAttackMotionDelegate(UPlayMontageCallbackProxy* Proxy, const FGameplayTag& Tag);
	
	UFUNCTION()
	virtual void OnAttackMotionEndplayMontage(FName NotifyName);
	
protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChooserTable> ChooserTable = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPlayMontageCallbackProxy> MontageProxy = nullptr;
	
	FGameplayTag AttackTag;
	
private:

	float ToPlayerDistance = 0.0f;
	
	UPROPERTY()
	TObjectPtr<UPJEnemyGroup> GroupManager = nullptr;
};

FORCEINLINE void APJEnemyBase::SetGroupManager(UPJEnemyGroup* Manager)
{
	GroupManager = Manager;
}

FORCEINLINE void APJEnemyBase::SetDistanceToPlayer(float Distance)
{
	ToPlayerDistance = Distance;
}

