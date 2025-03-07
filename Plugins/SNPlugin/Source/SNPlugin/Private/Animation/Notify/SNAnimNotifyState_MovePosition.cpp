// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotifyState_MovePosition.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "SNDef.h"
#include "Animation/Components/SNMovePositionComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
//#include "Types/AttributeStorage.h"

//----------------------------------------------------------------------//
//
//! @brief ノーティファイステートの開始
//
//! @param MeshComp       メッシュコンポーネント
//! @param Animation      再生中のアニメーション
//! @param TotalDuration  ノーティファイの持続時間
//! @param EventReference イベント情報
//
//----------------------------------------------------------------------//
void USNAnimNotifyState_MovePosition::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference){
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));
	
	if(Character == nullptr){
		return;
	}
	// 移動コンポーネントを取得
	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));
	
	if(MovePositionComponent != nullptr){
		// トランスフォーム情報を追加
		MovePositionComponent->AddTransform(TransformName, Character->GetActorTransform(), EventReference.GetCurrentAnimationTime());
	}
}

//----------------------------------------------------------------------//
//
//! @brief ノーティファイ中に呼ばれるティック処理
//
//! @param MeshComp       メッシュコンポーネント
//! @param Animation      再生中のアニメーション
//! @param FrameDeltaTime 経過時間
//! @param EventReference イベント情報
//
//----------------------------------------------------------------------//
void USNAnimNotifyState_MovePosition::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference){
	
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));
	
	if(Character == nullptr){
		return;
	}
	// 移動コンポーネントを取得
	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));
	
	if(MovePositionComponent != nullptr){
		
		const FTransformData* TransformData(MovePositionComponent->GetTransformData(TransformName));
		
		if(TransformData != nullptr){
			// ノーティファイステート内での再生時間を取得
			float ElapsedTime = EventReference.GetCurrentAnimationTime();
			// ノーティファイステートの終了時間を取得
			float EndTime = TransformData->StartTime + (EventReference.GetNotify() != nullptr) ? EventReference.GetNotify()->GetDuration() : 0.0f;
			// ノーティファイステートの何パーセントまで経過したかを算出(0.0 - 1.0)
			float DeltaRatio = CalculateNotifyStateProgressPercentage(TransformData->StartTime, EndTime, ElapsedTime);
			
			FVector Location(TransformData->StartTransform.GetLocation()), TmpLocation(FVector::ZeroVector);
			// 位置情報を移動するかチェック
			if(bMoveLocation == true){
				
				FVector CurveValue(DeltaRatio, DeltaRatio, DeltaRatio);
				
				if(TranslateInterpolate != nullptr){
					// カーブ情報から取得
					CurveValue = TranslateInterpolate->GetVectorValue(DeltaRatio);
				}
				
				if(bUsePositionCurveAsAbsoluteValue == true){
					TmpLocation = CurveValue;
				} else {
					TmpLocation.X = TargetPosition.X * CurveValue.X;
					TmpLocation.Y = TargetPosition.Y * CurveValue.Y;
					TmpLocation.Z = TargetPosition.Z * CurveValue.Z;
				}
				
				if(bAdditive == false){
					Location = TmpLocation;
				} else {
					Location = TransformData->StartTransform.GetLocation() + TmpLocation;
				}
			}
			
			FRotator Rotator(TransformData->StartTransform.GetRotation()), TmpRotation(FRotator::ZeroRotator);
			
			if(bMoveRotation == true){
				
				FVector CurveValue(FVector::ZeroVector);
				
				if(RotateInterpolate != nullptr){
					// カーブ情報から取得
					CurveValue = RotateInterpolate->GetVectorValue(DeltaRatio);
				}
				
				if(bUseRotationCurveAsAbsoluteValue == true){
					TmpRotation = FRotator(CurveValue.X, CurveValue.Y, CurveValue.Z);
				} else {
					TmpRotation.Pitch= TargetRotation.Pitch * CurveValue.X;
					TmpRotation.Yaw  = TargetRotation.Yaw   * CurveValue.Y;
					TmpRotation.Roll = TargetRotation.Roll  * CurveValue.Z;
				}
				
				if(bAdditive == false){
					Rotator = TmpRotation;
				} else {
					Rotator = TransformData->StartTransform.GetRotation().Rotator() + TmpRotation;
				}
				
			}
			
			FTransform Transform(FRotator(Rotator), Location);
			
			Character->SetActorTransform(Transform);
		}
	}
}

//----------------------------------------------------------------------//
//
//! @brief ノーティファイステートの終了
//
//! @param MeshComp       メッシュコンポーネント
//! @param Animation      再生中のアニメーション
//! @param EventReference イベント情報
//
//----------------------------------------------------------------------//
void USNAnimNotifyState_MovePosition::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference){
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ACharacter* Character(Cast<ACharacter>(MeshComp->GetOwner()));
	
	if(Character == nullptr){
		return;
	}
	// 移動コンポーネントを取得
	USNMovePositionComponent* MovePositionComponent(Cast<USNMovePositionComponent>(Character->GetComponentByClass(USNMovePositionComponent::StaticClass())));
	
	if(MovePositionComponent != nullptr){
		MovePositionComponent->RemoveTransform(TransformName);
	}
}

//----------------------------------------------------------------------//
//
//! @brief ノーティファイステートの進行度を0 - 1の比率に計算
//
//! @param StartTime   ノーティファイステートの開始時間
//! @param EndTime     ノーティファイステートの終了時間
//! @param CurrentTime 現在の経過時間
//
//! @retval 進行度
//
//----------------------------------------------------------------------//
float USNAnimNotifyState_MovePosition::CalculateNotifyStateProgressPercentage(float StartTime, float EndTime, float CurrentTime){
	// NotifyStateの総時間（終了時間 - 開始時間）
	float TotalDuration = EndTime - StartTime;
	// NotifyStateの開始時間からの経過時間
	float ElapsedTime = CurrentTime - StartTime;
	// パーセンテージを計算（0 ～ 1）
	float Percentage = (TotalDuration > 0) ? (ElapsedTime / TotalDuration) : 0.0f;
	// クラッピングして0%～100%の範囲内に保つ
	float Result = FMath::Clamp(Percentage, 0.0f, 1.0f);
	
	return Result;
}
