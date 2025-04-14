// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "PJSpawner.generated.h"

class USNMaterialComponent;
class AFieldSystemActor;
class APJEnemy;
class UCurveVector;

UCLASS()
class PJ0000_API APJSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	APJSpawner();

	UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnAnimationTimer();

	UFUNCTION()
	void OnFadeOut();
	
	UFUNCTION()
	void OnBreakEvent(const FChaosBreakEvent& BreakEvent);
	
	UFUNCTION()
	void OnAllEnemyDie();

	UFUNCTION()
	void OnEnemyGone();
	
	void Destruction();
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APJEnemy>> SpawnClass;
	
	UPROPERTY(EditAnywhere)
	int SpawnNum = 4;

	UPROPERTY(EditAnywhere)
	float SpawnRadius = 500.0f;
	
	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<UCurveVector> CurveVector = nullptr;

	UPROPERTY(EditAnywhere, Category=Animation)
	float AnimationTime = 0.0f;

	UPROPERTY(EditAnywhere, Category=FadeOut)
	float FadeOutTime = 2.0f;

	UPROPERTY(EditAnywhere, Category=Destruct)
	TSubclassOf<AFieldSystemActor> FieldSystemClass = nullptr;
	
	UPROPERTY(EditAnywhere, Category=Limit)
	int HitAndSpawnNum = 5;

	UPROPERTY(EditAnywhere, Category=Limit)
	int NoEnemyAndSpawnNum = 4;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USNMaterialComponent> MaterialComponent = nullptr;
	
	float AnimTimer = 0.0f;

	float FadeTimer = 0.0f;

	int CollideNum = 1;

	int SpawnCount = 0;

	UPROPERTY()
	TArray<AActor*> CollideActor;

	FTimerHandle AnimTimerHandle;

	FTimerHandle FadeTimerHandle;

	
};
