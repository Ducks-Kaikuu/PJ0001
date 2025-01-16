// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNMovePositionComponent.generated.h"

USTRUCT(BlueprintType)
struct FTransformData
{
	GENERATED_BODY()

	float TotalDuration = 0.0001f;
	
	FTransform SrcTransform = FTransform::Identity;

	FTransform DestTransform = FTransform::Identity;
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

	void AddTargetTransform(const FName& Name, const FTransform& Transform);

	void AddSourceTransform(const FName& Name, const FTransform& Transform);

	void SetStateTotalDuration(const FName& Name, float Duration);

	float GetStateTotalDuration(const FName& Name) const;

	const FTransform& GetDestTransform(const FName& Name) const ;

	const FTransform& GetSrcTransform(const FName& Name) const ;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	TMap<FName, FTransformData> TransformMap;
};
