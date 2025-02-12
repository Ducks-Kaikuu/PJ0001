// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SNAnimNotify_RemoveActionTag.generated.h"

//!@{@defgroup アニメーションノーティファイ
//!@{
//----------------------------------------------------------------------//
//
//! @brief アクションタグを削除
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNAnimNotify_RemoveActionTag : public UAnimNotify
{
	GENERATED_BODY()

public:
	//! @{@name ノーティファイ
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//! @}
	
private:
	
#if WITH_EDITORONLY_DATA
	//!< デバッグ情報出力フラグ
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDraw = false;
#endif
	//!< 削除するアクションタグ情報
	UPROPERTY(EditAnywhere, Category = "Action|Tag")
	FGameplayTagContainer Tags;
};
//! @}
//! @}
