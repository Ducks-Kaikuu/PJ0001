// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMatching/Environment/SNTraversalBlock.h"
#include "SNDef.h"
#include "Algo/Count.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASNTraversalBlock::ASNTraversalBlock()
:Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("RootComponent")));
	
	SNPLUGIN_ASSERT(SceneComponent != nullptr, TEXT("RootComponent is nullptr."));
	
	SetRootComponent(SceneComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SNPLUGIN_ASSERT(Mesh != nullptr, TEXT("StaticMesh is nullptr."));
	
	Mesh->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	
}

// Called when the game starts or when spawned
void ASNTraversalBlock::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	Ledges.Empty();
	
	TArray<TObjectPtr<UActorComponent>> SplineComponents(K2_GetComponentsByClass(USplineComponent::StaticClass()));
	
	for(TObjectPtr<UActorComponent> Component : SplineComponents)
	{
		USplineComponent* SplineComponent = Cast<USplineComponent>(Component);

		if(SplineComponent != nullptr)
		{
			Ledges.Add(SplineComponent);
		}
	}

	OppositeLedges.Empty();
	
	for(int i = 0; i < Ledges.Num(); i+=2)
	{
		int Index00 = i + 0;
		int Index01 = i + 1;

		if((Index00 >= Ledges.Num()) || (Index01 >= Ledges.Num()))
		{
			break;
		}

		USplineComponent* SplineComponent00(Ledges[Index00]);
		USplineComponent* SplineComponent01(Ledges[Index01]);

		OppositeLedges.Add(SplineComponent00, SplineComponent01);
		OppositeLedges.Add(SplineComponent01, SplineComponent00);
	}
}

void ASNTraversalBlock::GetLedgeTransform(FVector HitLocation, FVector ActorLocation, FTraversalCheckResult& TraversalTraceResult)
{
	USplineComponent* ClosestLedge(FindClosestLedgeToActor(ActorLocation));
	
	if(ClosestLedge == nullptr){
		
		TraversalTraceResult.HasFrontLedge = false;
		
		return;
	}
	
	if(ClosestLedge->GetSplineLength() >= MinLedgeWidth)
	{
		
	} else {
		
		TraversalTraceResult.HasFrontLedge = false;
		
		return;
	}
	
	FVector ClosestLocation(ClosestLedge->FindLocationClosestToWorldLocation(HitLocation, ESplineCoordinateSpace::Local));
	
	float Distance = ClosestLedge->GetDistanceAlongSplineAtLocation(ClosestLocation, ESplineCoordinateSpace::Local);
	
	Distance = FMath::Clamp(Distance, MinLedgeWidth * 0.5f, ClosestLedge->GetSplineLength() - MinLedgeWidth*0.5f);
	
	FTransform FrontLedgeTransform(ClosestLedge->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World, false));
	
	TraversalTraceResult.HasFrontLedge = true;
	
	TraversalTraceResult.FrontLedgeLocation = FrontLedgeTransform.GetLocation();
	
	TraversalTraceResult.FrontLedgeNormal = UKismetMathLibrary::GetUpVector(FrontLedgeTransform.Rotator());
	
	USplineComponent* OppositeLedge(OppositeLedges.Find(ClosestLedge) != nullptr ? OppositeLedges[ClosestLedge] : nullptr);
	
	if(OppositeLedge == nullptr){
		
		TraversalTraceResult.HasBackLedge = false;
		
		return;
	}
	
	FTransform BackLedgeTransform(OppositeLedge->FindTransformClosestToWorldLocation(TraversalTraceResult.FrontLedgeLocation, ESplineCoordinateSpace::World, false));
	
	TraversalTraceResult.HasBackLedge = true;
	
	TraversalTraceResult.BackLedgeLocation = BackLedgeTransform.GetLocation();
	
	TraversalTraceResult.BackLedgeNormal = UKismetMathLibrary::GetUpVector(BackLedgeTransform.Rotator());

}

USplineComponent* ASNTraversalBlock::FindClosestLedgeToActor(const FVector& ActorLocation)
{
	if(Ledges.IsEmpty() == true){
		return nullptr;
	}
	
	float ClosestDistance = 0.0f;
	
	int ClosestIndex = 0, Count = 0;
	
	for(USplineComponent* SplineComponent : Ledges){
		// アクターのロケーションから最も近いスプライン曲線上の点を取得
		FVector ClosestLocation(SplineComponent->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World));
		// アクターのロケーションから最も近い曲線のアップベクトルを取得
		FVector ClosestUpVector(SplineComponent->FindUpVectorClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World));
		// アクターとの距離を算出
		float Distance = FVector::Distance(ClosestLocation + ClosestUpVector * 10.0f, ActorLocation);
		// 最も近い曲線かチェック
		if((Distance < ClosestDistance) || (Count == 0)){
			
			ClosestDistance = Distance;
			
			ClosestIndex = Count;
		}
		
		++Count;
	}
	
	return Ledges[ClosestIndex];
}
