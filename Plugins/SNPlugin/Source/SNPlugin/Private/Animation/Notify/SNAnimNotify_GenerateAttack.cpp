﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotify_GenerateAttack.h"

#include "Character/Components/SNAttackComponent.h"

void USNAnimNotify_GenerateAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference){
	
	Super::Notify(MeshComp, Animation, EventReference);
	// 攻撃コリジョンの発生位置を設定
	FVector StartPos = (bSweep == true) ? Start : Position;
	FVector EndPos =   (bSweep == true) ? End   : Position;
	// 親のソケットが存在するかチェック
	if(ParentSocket != NAME_None){
		// コンストするソケットのトランスフォーム情報を取得
		FTransform Transform(MeshComp->GetSocketTransform(ParentSocket));
		// 親ソケットのローテーション情報を無視するかチェック
		if(bIgnoreRotation == true){
			// オーナーのアクターを取得
			AActor* OwnerActor = Cast<AActor>(MeshComp->GetOwner());
			// アクターのローテーション情報を取得
			FRotator Rotator(OwnerActor->GetActorRotation());
			// ソケットではなくオーナーのローテーション情報を使用
			Transform.SetRotation(Rotator.Quaternion());
		}
		// 位置情報をワールドに変換
		StartPos = Transform.TransformPosition(StartPos);
		EndPos   = Transform.TransformPosition(EndPos);
	}
	
	AActor* Attacker = Cast<AActor>(MeshComp->GetOwner());
	
	if(Attacker != nullptr){
		// 攻撃コンポーネントを取得
		USNAttackComponent* AttackComponent = Attacker->FindComponentByClass<USNAttackComponent>();
		
		if(AttackComponent != nullptr){

			AttackComponent->GenerateAttackGuid();
			
			TArray<AActor*> IgnoreList({Attacker});
			// コリジョンを生成
			AttackComponent->GenerateSphereSweep(Attacker, StartPos, EndPos, Radius, DamageAttribetes, IgnoreList, bPenetrate, bBomb, AttackComponent->GetAttackGuid());
		}
	}
}
