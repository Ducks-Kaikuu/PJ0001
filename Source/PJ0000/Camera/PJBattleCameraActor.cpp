// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/PJBattleCameraActor.h"

#include "Character/Base/SNPlayerBase.h"
#include "Utility/SNUtility.h"

void APJBattleCameraActor::BeginPlay()
{
	Super::BeginPlay();

	ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

	AttachToActor(Player, FAttachmentTransformRules::KeepRelativeTransform, BoneName);

	FTransform OffsetTransform(FTransform::Identity);

	FVector Offset(TargetOffset);

	Offset.Z += FocuedCameraHeight;
	
	OffsetTransform.SetTranslation(Offset);

	SetActorTickEnabled(true);

}

void APJBattleCameraActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCameraTick == false)
	{
		return;
	}

	ASNPlayerBase* Player(SNUtility::GetCurrentPlayer<ASNPlayerBase>());

	if(Player != nullptr){
#if 0
		static bool isInitialize = false;

		static FTransform Test;

		if(isInitialize == false)
		{
			Test = (BoneName != NAME_None ? Player->GetMesh()->GetBoneTransform(BoneName) : Player->GetActorTransform());

			isInitialize = true;
		}

		FTransform Transform(Test);
#else
		// アタッチするトランスフォームの情報を取得
		FTransform Transform(BoneName != NAME_None ? Player->GetMesh()->GetBoneTransform(BoneName) : Player->GetActorTransform());
#endif
		
		
		if(bCarFocusedCamera == true){
			// アクターと向きが違うので180度回転
			const FRotator ReverseRotation(0, 180, 0);
			// ターゲットからのオフセットを回転
			FVector Offset(ReverseRotation.RotateVector(TargetOffset));
			// カメラのローテーションを反映
			FVector CameraPosition(CameraRotate.RotateVector(Offset));
			// フォーカス時の高さを加算
			Transform.AddToTranslation(FVector(0.0f, 0.0f, FocuedCameraHeight));
			// 親トランスフォームを反映
			CameraPosition = Transform.TransformPosition(CameraPosition);
			// ターゲットへのベクトルを算出
			FVector Vector(Transform.GetLocation() - CameraPosition);
			// ベクトルをノーマライズ
			bool bSucceed = Vector.Normalize();
			
			if(bSucceed == true){
				// カメラのトランスフォームを生成
				FTransform CameraTransform(Vector.Rotation(), CameraPosition);
				// トランスフォームを設定
				SetActorTransform(CameraTransform);
			} else {
				
				FTransform CameraTransform(FQuat::Identity, CameraPosition);
				
				SetActorTransform(CameraTransform);
			}
			
		} else {
			
			const FRotator ReverseRotation(0, 180, 0);
			
			FVector Offset(ReverseRotation.RotateVector(TargetOffset));
			
			FTransform CameraTransform(CameraRotate, Offset);
			
			CameraTransform = CameraTransform * Transform;
			
			SetActorTransform(CameraTransform);
		}
	}
}
