// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/PJSpawner.h"

#include "SNDef.h"
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

	StaticMesh->SetNotifyRigidBodyCollision(true);
	
	//StaticMesh->OnComponentHit.AddDynamic(this, &APJSpawner::OnComponentHit);

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APJSpawner::OnComponentBeginOverlap);

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

void APJSpawner::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APJEnemy* Enemy = Cast<APJEnemy>(OtherActor);

	if (Enemy != nullptr)
	{
		if (CollideActor.Contains(Enemy) == false)
		{
			if (Enemy->GetActionTags().HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.State.Damage")))))
			{
				if (TimerHandle.IsValid() == false)
				{
					GetWorldTimerManager().SetTimer(TimerHandle, this, &APJSpawner::OnAnimationTimer, 1.0f/60.0f, true);

					CollideActor.Add(Enemy);

					SNPLUGIN_LOG(TEXT("APJSpawner::OnComponentBeginOverlap : %d"), CollideActor.Num());
				}
			}
		}
	}
}

void APJSpawner::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APJEnemy* Enemy = Cast<APJEnemy>(OtherActor);

	if (Enemy != nullptr)
	{
		if (CollideActor.Contains(Enemy) == false)
		{
			if (Enemy->GetActionTags().HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.State.Damage")))))
			{
				if (TimerHandle.IsValid() == false)
				{
					GetWorldTimerManager().SetTimer(TimerHandle, this, &APJSpawner::OnAnimationTimer, 1.0f/60.0f, true);
				}

				CollideActor.Add(Enemy);

				SNPLUGIN_LOG(TEXT("APJSpawner::OnComponentHit : %d"), CollideActor.Num());
			}
		}
	}
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

			for (int i= 0 ; i<CollideActor.Num() ; i++)
			{
				OnEnemyGone();	
			}

			CollideActor.Empty();
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
		SpawnEnemy(SpawnLocation);
	}
}

AActor* APJSpawner::SpawnEnemy(const FVector& SpawnLocation)
{
	FVector Range(FMath::RandRange(0.0f, SpawnRadius * 2.0f) - SpawnRadius , FMath::RandRange(0.0f, SpawnRadius * 2.0f) - SpawnRadius, 88.0f);

	if (Range.X < 0.0f)
	{
		Range.X = FMath::Clamp(Range.X, -SpawnRadius, -250.0f);
	} else
	{
		Range.X = FMath::Clamp(Range.X, 250.0f, SpawnRadius);
	}

	if (Range.Y < 0.0f)
	{
		Range.Y = FMath::Clamp(Range.Y, -SpawnRadius, -250.0f);
	} else
	{
		Range.Y = FMath::Clamp(Range.Y, 250.0f, SpawnRadius);
	}

	FVector Location = SpawnLocation + Range;

	TSubclassOf<APJEnemy>& EnemyClass = SpawnClass[static_cast<int>(FMath::RandRange(0.0f, static_cast<float>(SpawnClass.Num()-1)))];

	FTransform SpawnTransform = FTransform::Identity;

	SpawnTransform.SetLocation(Location);
		
	return GetWorld()->SpawnActor(EnemyClass.Get(), &SpawnTransform);
}
