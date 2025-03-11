// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/PJPlayerCameraManager.h"

#include "PJCameraActorBase.h"
#include "SNDef.h"

APJPlayerCameraManager::APJPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
,CurrentCamera(NAME_None)
{
}

void APJPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World(GetWorld());
	
	if(World != nullptr){
		
		for(auto Object: CameraClassMap){
			
			APJCameraActorBase* CameraInstance = nullptr;
			
			UClass* Class(Object.Value.LoadSynchronous());
			
			if(Class != nullptr){
				
				CameraInstance = Cast<APJCameraActorBase>(World->SpawnActor(Class));
				
				CameraInstanceMap.Add(Object.Key, CameraInstance);
				
				if(CurrentCamera == NAME_None)
				{
					CurrentCamera = Object.Key;
				}
			}
			
			SNPLUGIN_ASSERT(CameraInstance != nullptr, TEXT("Failed to create camera instance : %s"), *Object.Key.ToString());
		}
	}

	if (CurrentCamera !=NAME_None)
	{
		SetViewTarget(CameraInstanceMap[CurrentCamera]);
	}
}

void APJPlayerCameraManager::SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	APlayerController* PlayerController = GetOwningPlayerController();

	APJCameraActorBase* Camera(Cast<APJCameraActorBase>(NewViewTarget));
	// Cameraのベースクラスにキャストできない場合はターゲットに設定しない。
	// 初期化の順番によってPawnなどもここに飛んでくるため。
	if((PlayerController != nullptr) && (Camera != nullptr))
	{
		Super::SetViewTarget(NewViewTarget, TransitionParams);
	}
}
