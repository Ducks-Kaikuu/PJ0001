// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SNAnimNotify_GenerateAttack.generated.h"

//!@{@defgroup アニメーションノーティファイ
//!@{
//----------------------------------------------------------------------//
//
//! @brief 攻撃コリジョン生成
//
//----------------------------------------------------------------------//
UCLASS()
class SNPLUGIN_API USNAnimNotify_GenerateAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	//! @{@name ノーティファイ
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//! @}
	
	//!< 親となるソケットorボーン名
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	FName ParentSocket = NAME_None;
	
	//!< 親のローテーション情報を無理するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bIgnoreRotation = false;

	//!< スイープで判定するかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bSweep = false;

	//!< 貫通させるかのフラグ
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bPenetrate = false;

	//!< 爆発属性か…(現在未使用)
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	bool bBomb = false;

	//!< ダメージ時間
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	float Time = 0.0f;

	//!< 攻撃の大きさ
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	float Radius = 5.0f;

	//!< ポジション情報
	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "!bSweep"))
	FVector Position;

	//!< スイープで判定する場合の開始地点
	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "bSweep"))
	FVector Start;

	//!< スイープで判定する場合の終了地点
	UPROPERTY(EditAnywhere, Category = "SN|Attack", meta = (EditCondition = "bSweep"))
	FVector End;

	//!< ダメージ属性
	UPROPERTY(EditAnywhere, Category = "SN|Attack")
	FGameplayTagContainer DamageAttribetes;
};
//! @}
//! @}
