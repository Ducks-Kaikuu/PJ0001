// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/PJSpawner.h"

#include "SNDef.h"
#include "Character/Components/SNMaterialComponent.h"
#include "Character/Enemy/PJEnemy.h"
#include "Character/Enemy/PJEnemyGroup.h"
#include "Character/Enemy/PJEnemyManager.h"
#include "Curves/CurveVector.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "System/PJGameInstance.h"
#include "Utility/SNUtility.h"

// Sets default values
//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
APJSpawner::APJSpawner(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PrimaryActorTick.bStartWithTickEnabled = false;
	//破壊コンポーネントを生成
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollectionComponent");
	
	if(GeometryCollectionComponent != nullptr){
		
		SetRootComponent(GeometryCollectionComponent);
		
		TArray<float> DamageThreshold;
		
		DamageThreshold.Add(99999.0f);
		// ダメージの閾値を設定
		GeometryCollectionComponent->SetDamageThreshold(DamageThreshold);
		// オーバーラップイベントを有効化(これをtrueにしないとオーバーラップイベントが発生しない)
		GeometryCollectionComponent->SetGenerateOverlapEvents(true);
		// オーバーラップイベントを設定
		GeometryCollectionComponent->OnComponentBeginOverlap.AddDynamic(this, &APJSpawner::OnComponentBeginOverlap);
	}
	// マテリアル操作用のコンポーネントを生成
	MaterialComponent = CreateDefaultSubobject<USNMaterialComponent>("MaterialComponent");
}

// Called when the game starts or when spawned
void APJSpawner::BeginPlay(){
	
	Super::BeginPlay();
	// Tickを無効化
	SetActorTickEnabled(false);
	
	if(MaterialComponent != nullptr){
		// ダイナミックマテリアルインスタンスを生成
		MaterialComponent->CreateMaterialInstanceDynamic();
	}
	
	UPJGameInstance* GameInstance = SNUtility::GetGameInstance<UPJGameInstance>();
	
	if(GameInstance != nullptr){
		
		UPJEnemyManager* EnemyManager = GameInstance->GetEnemyManager();
		
		if(EnemyManager != nullptr){
			// 敵が全滅した場合のデリゲートを登録
			EnemyManager->OnEnemyGone.AddUObject(this, &APJSpawner::OnAllEnemyDie);
		}
	}
}

void APJSpawner::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APJEnemy* Enemy = Cast<APJEnemy>(OtherActor);
	
	if(Enemy != nullptr){
		
		if(CollideActor.Contains(Enemy) == false){
			
			if(Enemy->GetActionTags().HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.State.Damage"))))
			|| Enemy->GetActionTags().HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Abilities.Damage"))))){
				
				if(AnimTimerHandle.IsValid() == false){
					// スポナーにぶつかったことがわかるようにアニメーションを生成
					GetWorldTimerManager().SetTimer(AnimTimerHandle, this, &APJSpawner::OnAnimationTimer, 1.0f/60.0f, true);
					
					SNPLUGIN_LOG(TEXT("APJSpawner::OnComponentBeginOverlap : %d"), CollideActor.Num());
				}
				// ぶつかった敵を登録
				CollideActor.Add(Enemy);
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
		Destruction();
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
			// スポナーにヒットしたアクター数分敵をスポーン
			for(int i= 0 ; i<CollideActor.Num() ; i++){
				OnEnemyGone();
			}
			
			CollideActor.Empty();
			
			--HitAndSpawnNum;
			
			if(HitAndSpawnNum <= 0){
				Destruction();
			}
		}

		SetActorScale3D(Scale);
	}
}

void APJSpawner::Destruction(){
	
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
	
	UPJEnemyGroup* Group = NewObject<UPJEnemyGroup>(this, UPJEnemyGroup::StaticClass());

	if (Group != nullptr)
	{
		FVector SpawnLocation = GetActorLocation();
		
		TSubclassOf<APJEnemy>& EnemyClass = SpawnClass[static_cast<int>(FMath::RandRange(0.0f, static_cast<float>(SpawnClass.Num()-1)))];
		
		Group->SpawnGroupsEnemy(EnemyClass, SpawnNum, SpawnLocation, SpawnRadius);
	}
}
