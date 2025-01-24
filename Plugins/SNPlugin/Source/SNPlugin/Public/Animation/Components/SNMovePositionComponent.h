// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNMovePositionComponent.generated.h"

USTRUCT(BlueprintType)
struct FTransformData
{
	GENERATED_BODY()
	// 移動開始時のトランスフォーム情報
	FTransform StartTransform = FTransform::Identity;
	// 合成開始時間
	float StartTime = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNMovePositionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNMovePositionComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddTransform(const FName& Name, const FTransform& Transform, float StartTime);
	
	const FTransformData* GetTransformData(const FName& Name) const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	TMap<FName, FTransformData> TransformMap;
};
