// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Action/PJCameraAction.h"

#include "Camera/PJBattleCameraActor.h"
#include "Character/Base/SNPlayerBase.h"
#include "Utility/SNUtility.h"

bool UPJCameraAction::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	const FInputActionValue::Axis2D Axis(InputActionValue.Get<FInputActionValue::Axis2D>());
	// 現状のカメラを取得
	APJBattleCameraActor* CameraActor(SNUtility::GetCurrentCameraActor<APJBattleCameraActor>());
	// バトルカメラ出ない場合は処理をしない
	if(CameraActor != nullptr){
		
		float Yaw  (FMath::Abs(Axis.X) < 0.05f ? 0.0f : Axis.X);
		float Pitch(FMath::Abs(Axis.Y) < 0.05f ? 0.0f : Axis.Y);
		
		Yaw   *= CameraActor->GetXAxisSpeed();
		Pitch *= CameraActor->GetYAxisSpeed();
		
		FRotator rot(CameraActor->GetCameraRotator());
		
		rot.Yaw   += Yaw;
		rot.Pitch += Pitch;
		
		rot.Pitch = FMath::Clamp(rot.Pitch, -60.0f, 75.0);
		
		CameraActor->SetCameraRotator(rot);

		ASNPlayerBase* Player(GetOwner<ASNPlayerBase>());

		if(Player != nullptr)
		{
			Player->AddControllerYawInput(Yaw);

			Player->AddControllerPitchInput(Pitch);
		}
	}

	return true;
}
