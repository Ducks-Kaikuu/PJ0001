// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "PJEqsManager.generated.h"

class USNEqsLocationList;

#if 0
USTRUCT()
struct FEqsRequestInstance
{
	GENERATED_BODY()

	FEqsRequestInstance();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = EQS)
	bool bEqsDebugDraw = false;
#endif
	
	UPROPERTY(EditAnywhere, Category = EQS)
	FEQSParametrizedQueryExecutionRequest EQSRequest;

	UPROPERTY(EditAnywhere, Category = EQS)
	TObjectPtr<USNEqsLocationList> EqsLocationList = nullptr;

	FQueryFinishedSignature OnQueryFinishedEvent;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult);
};
#else
UCLASS(Blueprintable)
class UEQSRequestInstance : public UObject
{
	GENERATED_BODY()

public:

	UEQSRequestInstance();

	virtual void PostLoad() override;

	friend class UPJEqsManager;
	
private:

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = EQS)
	bool bEqsDebugDraw = false;
#endif
	
	UPROPERTY(EditAnywhere, Category = EQS)
	FEQSParametrizedQueryExecutionRequest EQSRequest;

	UPROPERTY(EditAnywhere, Category = EQS)
	TObjectPtr<USNEqsLocationList> EqsLocationList = nullptr;

	FQueryFinishedSignature OnQueryFinishedEvent;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult);
};
#endif
/**
 * 
 */
UCLASS(Blueprintable)
class PJ0000_API UPJEqsManager : public UObject
{
	GENERATED_BODY()

public:

	void Initialize();

	void RunEqs(UWorld* World, const FGameplayTagContainer& EqsTags);

	USNEqsLocationList* GetEqsLocationList(const FGameplayTag& EqsTag);
	
private:
#if 0
	UPROPERTY(EditAnywhere, Category = "EQS|Data")
	TMap<FGameplayTag, FEqsRequestInstance> EqsMap; 
#else
	UPROPERTY(EditAnywhere, Category = "EQS|Data")
	TMap<FGameplayTag, TSoftClassPtr<UEQSRequestInstance>> EqsMap;


	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UEQSRequestInstance>> EQSInstanceMap;
#endif
};
