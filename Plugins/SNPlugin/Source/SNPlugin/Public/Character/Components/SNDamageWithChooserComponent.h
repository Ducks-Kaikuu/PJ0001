// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNDamageWithChooserComponent.generated.h"


class UChooserTable;
class UPlayMontageCallbackProxy;
struct FGameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNDamageWithChooserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNDamageWithChooserComponent();

	UPlayMontageCallbackProxy* PlayDamageAnimation(const FGameplayTagContainer& DamageTags, bool bAddToOwner=true);

	virtual void Death(){}
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	TObjectPtr<UChooserTable> DamageAnimationChooser = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "SN|Damage")
	bool bDebugDraw = false;
#endif
	
};
