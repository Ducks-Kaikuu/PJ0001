// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/PJSpawner.h"

#include "SNDef.h"
#include "Character/Components/SNMaterialComponent.h"
#include "Character/NPC/PJEnemy.h"
#include "Character/NPC/PJEnemyManager.h"
#include "Curves/CurveVector.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "System/PJGameInstance.h"
#include "Utility/SNUtility.h"

// Sets default values
APJSpawner::APJSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollectionComponent");

	if (GeometryCollectionComponent != nullptr)
	{
		SetRootComponent(GeometryCollectionComponent);

		TArray<float> DamageThreshold;

		DamageThreshold.Add(99999.0f);

		GeometryCollectionComponent->SetGenerateOverlapEvents(true);
		
		GeometryCollectionComponent->SetDamageThreshold(DamageThreshold);
		
		GeometryCollectionComponent->OnComponentBeginOverlap.AddDynamic(this, &APJSpawner::OnComponentBeginOverlap);
	}

	MaterialComponent = CreateDefaultSubobject<USNMaterialComponent>("MaterialComponent");
}

// Called when the game starts or when spawned
void APJSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (MaterialComponent != nullptr)
	{
		MaterialComponent->CreateMaterialInstanceDynamic();
	}

	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();

	if (GameInstance != nullptr)
	{
		UPJEnemyManager* EnemyManager = GameInstance->GetEnemyManager();

		if (EnemyManager != nullptr)
		{
			EnemyManager->OnEnemyGone.AddUObject(this, &APJSpawner::OnAllEnemyDie);
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
				if (AnimTimerHandle.IsValid() == false)
				{
					GetWorldTimerManager().SetTimer(AnimTimerHandle, this, &APJSpawner::OnAnimationTimer, 1.0f/60.0f, true);

					CollideActor.Add(Enemy);

					SNPLUGIN_LOG(TEXT("APJSpawner::OnComponentBeginOverlap : %d"), CollideActor.Num());
				}
			}
		}
	}
}

void APJSpawner::OnAllEnemyDie()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	
	OnEnemyGone();

	--NoEnemyAndSpawnNum;

	if (NoEnemyAndSpawnNum <= 0)
	{
		if (GeometryCollectionComponent != nullptr)
		{
			GeometryCollectionComponent->SetSimulatePhysics(true);

			GeometryCollectionComponent->AddImpulse(FVector(0, 0, 0));

			GeometryCollectionComponent->SetNotifyBreaks(true);
			
			GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &APJSpawner::OnBreakEvent);
			
			GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
			GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

			FVector Locate = GetActorLocation();
			
			AFieldSystemActor* FieldActor = Cast<AFieldSystemActor>(GetWorld()->SpawnActor(FieldSystemClass, &Locate));
			
			//FieldActor->Destroy();
		}
	}
}

void APJSpawner::OnAnimationTimer()
{
	const float FPS = 1.0f/60.0f;

	if (CurveVector != nullptr)
	{
		FVector Scale = CurveVector->GetVectorValue(AnimTimer);

		AnimTimer += FPS;

		if (AnimTimer >= AnimationTime)
		{
			Scale = FVector::OneVector;
			
			GetWorldTimerManager().ClearTimer(AnimTimerHandle);

			AnimTimer = 0.0f;

			AnimTimerHandle.Invalidate();

			for (int i= 0 ; i<CollideActor.Num() ; i++)
			{
				OnEnemyGone();	
			}

			HitAndSpawnNum =- 1;
			
			CollideActor.Empty();
		}

		SetActorScale3D(Scale);
	}
}

void APJSpawner::OnFadeOut()
{
	FadeTimer += 0.016f;

	float Alpha = FadeTimer / FadeOutTime;

	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

	MaterialComponent->SetScalarParameterValue(NAME_None, TEXT("DitherAlpha"), 1.0f - Alpha);

	if (FadeTimer >= FadeOutTime){

		GetWorldTimerManager().ClearTimer(FadeTimerHandle);

		Destroy();
	}
}

void APJSpawner::OnBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	if (GeometryCollectionComponent != nullptr)
	{
		GeometryCollectionComponent->SetSimulatePhysics(false);

		GeometryCollectionComponent->SetCollisionObjectType(ECC_WorldStatic);

		GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &APJSpawner::OnFadeOut, 0.016f, true);
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
