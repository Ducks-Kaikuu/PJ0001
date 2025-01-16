// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionMatching/Traversal/TraversalCheckResult.h"
#include "SNTraversalBlock.generated.h"

class USplineComponent;

UCLASS()
class SNANIMATION_API ASNTraversalBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNTraversalBlock();
	
	UFUNCTION(BlueprintCallable, Category = "SN|Traversal")
	void GetLedgeTransform(FVector HitLocation, FVector ActorLocation, UPARAM(ref) FTraversalCheckResult& TraversalTraceResult);

	UFUNCTION(BlueprintCallable, Category = "SN|Traversal")
	UStaticMeshComponent* GetMeshComponent();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	USplineComponent* FindClosestLedgeToActor(const FVector& ActorLocation);
	
	UPROPERTY()
	TArray<TObjectPtr<USplineComponent>> Ledges;

	UPROPERTY()
	TMap<TObjectPtr<USplineComponent>, TObjectPtr<USplineComponent>> OppositeLedges;

	UPROPERTY(EditAnywhere, Category="SN|Traversal")
	float MinLedgeWidth = 60.0f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh=nullptr;

};

FORCEINLINE UStaticMeshComponent* ASNTraversalBlock::GetMeshComponent()
{
	return Mesh;
}