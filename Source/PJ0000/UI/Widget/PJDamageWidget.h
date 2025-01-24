// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/SNUserWidgetBase.h"
#include "PJDamageWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PJ0000_API UPJDamageWidget : public USNUserWidgetBase
{
	GENERATED_BODY()

public:
	
	UPJDamageWidget();

	virtual bool Initialize() override;

	void PlayDamage(int DamageValue, FVector2D Position);

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DamageAnim = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Damage = nullptr;
};
