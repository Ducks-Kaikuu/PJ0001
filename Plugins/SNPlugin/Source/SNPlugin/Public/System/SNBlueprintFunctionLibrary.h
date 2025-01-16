// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SNBlueprintFunctionLibrary.generated.h"

class UChooserTable;
class USNOnlineSystem;

/**
 * 
 */
UCLASS()
class SNPLUGIN_API USNBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="SN|Input")
	static void SetInputMappingContext(APlayerController* PlayerController, FName Name, bool bClear=true);
		
	UFUNCTION(BlueprintCallable, Category="SN|Online")
	static void Login();

	UFUNCTION(BlueprintCallable, Category="SN|Online")
	static void HostSession();
	
	UFUNCTION(BlueprintCallable, Category="SN|Online")
	static void FindSession();

	UFUNCTION(BlueprintCallable, Category="SN|Online")
	static void KillSession(FName SessionName);

	UFUNCTION(BlueprintCallable, Category="SN|Online")
	static USNOnlineSystem* GetOnlineSystem();

	UFUNCTION(BlueprintCallable, Category="SN|Motion Matching")
	static UAnimMontage* GetAnimMontageFromChooser(ACharacter* Character, UChooserTable* ChooserTable, UObject* Parameter, UPARAM(ref) float& PlayRate, UPARAM(ref) float& StartTime);
};
