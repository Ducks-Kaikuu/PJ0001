// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PJDamageWidget.h"

#include "SNDef.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"


UPJDamageWidget::UPJDamageWidget()
{
		
}

bool UPJDamageWidget::Initialize()
{
	bool Result = Super::Initialize();

	//DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Damage")));

	//if (WidgetTree != nullptr)
	//{
	//	TArray<UWidget*> Widgets;
		
	//	WidgetTree->GetAllWidgets(Widgets);

	//	for (auto& Widget : Widgets)
	//	{
			//UWidgetAnimation* Animation = Cast<UWidgetAnimation>(Widget);

			//if (Animation != nullptr)
	//		{
				
	//		}
	//	}
	//}
	
	return Result;
}

void UPJDamageWidget::PlayDamage(int DamageValue, FVector2D Position)
{
	if (Damage != nullptr)
	{
		FText DamageText = FText::FromString(FString::FromInt(DamageValue));

		Damage->SetText(DamageText);

		Damage->SetRenderTranslation(Position);

		PlayAnimation(DamageAnim);
	}
}

void UPJDamageWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	SetVisibility(ESlateVisibility::Hidden);

	//RemoveFromParent();

	SNPLUGIN_LOG(TEXT("DamageWidget::OnAnimationEnded"));
}
