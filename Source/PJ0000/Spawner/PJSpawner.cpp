// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/PJSpawner.h"

#include "Character/NPC/PJEnemy.h"
#include "Character/NPC/PJEnemyManager.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/PJGameInstance.h"
#include "Utility/SNUtility.h"

// Sets default values
APJSpawner::APJSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;


	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	StaticMesh->SetupAttachment(GetRootComponent());

	StaticMesh->OnComponentHit.AddDynamic(this, &APJSpawner::OnComponentHit);

}

// Called when the game starts or when spawned
void APJSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance != nullptr)
	{
		UPJEnemyManager* EnemyManager = GameInstance->GetEnemyManager();

		if (EnemyManager != nullptr)
		{
			EnemyManager->OnEnemyGone.AddUObject(this, &APJSpawner::OnEnemyGone);
		}
	}
}

void APJSpawner::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APJEnemy* Enemy = Cast<APJEnemy>(OtherActor);

	if (Enemy != nullptr)
	{
		if (Enemy->GetActionTags().HasTag(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage.L"))))
		{
			
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APJSpawner::OnAnimationTimer, 1.0f/60.0f, true);
}

void APJSpawner::OnAnimationTimer()
{
	const float FPS = 1.0f/60.0f;

	if (CurveVector != nullptr)
	{
		FVector Scale = CurveVector->GetVectorValue(Timer);

		Timer += FPS;

		if (Timer >= AnimationTime)
		{
			Scale = FVector::OneVector;
			
			GetWorldTimerManager().ClearTimer(TimerHandle);

			Timer = 0.0f;

			TimerHandle.Invalidate();
		}

		SetActorScale3D(Scale);
	}
}

void APJSpawner::OnEnemyGone()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	
	FVector SpawnLocation = GetActorLocation();
	
	for (int i=0 ; i<SpawnNum ; ++i)
	{
		FVector Range(FMath::RandRange(2.0f, 5.0f) * 100.0f, FMath::RandRange(2.0f, 5.0f) * 100.0f, 88.0f);

		FVector Location = SpawnLocation + Range;

		TSubclassOf<APJEnemy>& EnemyClass = SpawnClass[(int)FMath::RandRange(0.0f, (float)SpawnClass.Num()-1)];

		FTransform SpawnTransform = FTransform::Identity;

		SpawnTransform.SetLocation(Location);
		
		GetWorld()->SpawnActor(EnemyClass.Get(), &SpawnTransform);
	}
	
}
