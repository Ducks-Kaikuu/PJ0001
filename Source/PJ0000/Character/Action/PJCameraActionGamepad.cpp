// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Action/PJCameraActionGamepad.h"

#include "Camera/PJBattleCameraActor.h"
#include "Character/Base/SNPlayerBase.h"
#include "Utility/SNUtility.h"

bool UPJCameraActionGamepad::ExecAction(const FInputActionValue& InputActionValue)
{
	Super::ExecAction(InputActionValue);

	const FInputActionValue::Axis2D Axis(InputActionValue.Get<FInputActionValue::Axis2D>());
	// 現状のカメラを取得
	APJBattleCameraActor* CameraActor(SNUtility::GetCurrentCameraActor<APJBattleCameraActor>());
	// バトルカメラ出ない場合は処理をしない
	if(CameraActor != nullptr){

		FVector2D Vector(InputActionValue[0], InputActionValue[1]);
		
		Vector *= UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		
		float Yaw  (FMath::Abs(Vector.X) < 0.05f ? 0.0f : Vector.X);
		float Pitch(FMath::Abs(Vector.Y) < 0.05f ? 0.0f : Vector.Y);
		
		Yaw   *= CameraActor->GetXAxisSpeed();
		Pitch *= CameraActor->GetYAxisSpeed();
		
		FRotator rot(CameraActor->GetCameraRotator()), aaa(rot);
		
		rot.Yaw   += Yaw;
		rot.Pitch += Pitch;
		
		aaa.Pitch = FMath::Clamp(rot.Pitch, -10.0f, 45.0);
		
		CameraActor->SetCameraRotator(aaa);

		ASNPlayerBase* Player(GetOwner<ASNPlayerBase>());

		if(Player != nullptr)
		{
			Player->AddControllerYawInput(rot.Yaw);

			//Player->AddControllerPitchInput(rot.Pitch);
		}
	}

	return true;
}
