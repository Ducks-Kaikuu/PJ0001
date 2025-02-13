// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNMaterialComponent.generated.h"

USTRUCT(BlueprintType)
struct FSNMaterial
{
	GENERATED_BODY()

	FSNMaterial():SourceInterface(nullptr), MaterialInstanceDynamic(nullptr){}
	
	FSNMaterial(UMaterialInterface* Source, UMaterialInstanceDynamic* Destination):
	SourceInterface(Source),
	MaterialInstanceDynamic(Destination)
	{}

	UPROPERTY()
	TObjectPtr<UMaterialInterface> SourceInterface = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USNMaterialComponent();

	void CreateMaterialInstanceDynamic();

	void SetScalarParameterValue(const FName& SlotName, const FName& ParamName, float Value);

	void SetTextureParameterValue(const FName& SlotName, const FName& ParamName, UTexture* Texture);

	void SetRuntimeVirtualTextureParameterValue(const FName& SlotName, const FName& ParamName, URuntimeVirtualTexture* Texture);

	void SetVectorParameterValue(const FName& SlotName, const FName& ParamName, const FVector& Value);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
		
	UPROPERTY()
	TMap<FName, FSNMaterial> Materials;
};
