// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PJSpawner.generated.h"

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
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnAnimationTimer();

	UFUNCTION()
	void OnEnemyGone();

	AActor* SpawnEnemy(const FVector& SpawnLocation);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

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

	float Timer = 0.0f;

	int CollideNum = 1;

	TArray<AActor*> CollideActor;

	FTimerHandle TimerHandle;
};

FORCEINLINE UStaticMeshComponent* APJSpawner::GetMesh()
{
	return StaticMesh;
}
