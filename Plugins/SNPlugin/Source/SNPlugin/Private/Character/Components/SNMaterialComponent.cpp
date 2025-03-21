// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/SNMaterialComponent.h"

#include "SNDef.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USNMaterialComponent::USNMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}

void USNMaterialComponent::CreateMaterialInstanceDynamic()
{
	AActor* Character = Cast<AActor>(GetOwner());

	if (Character != nullptr)
	{
		UMeshComponent* MeshComponent = Character->FindComponentByClass<UMeshComponent>();

		SNPLUGIN_ASSERT(MeshComponent != nullptr, TEXT("USMaterialComponent::BeginPlay: Mesh is null"));

		int Index = 0;

		const TArray<FName> SlotNames(MeshComponent->GetMaterialSlotNames());
		
		for (auto& Material : MeshComponent->GetMaterials())
		{
			if (Material != nullptr)
			{
				UMaterialInstanceDynamic* InstanceDynamic = MeshComponent->CreateDynamicMaterialInstance(Index);

				if (InstanceDynamic != nullptr)
				{
					FSNMaterial MaterialInfo(Material, InstanceDynamic);

					FName SlotName(NAME_None);
					// マテリアルが単体の場合、SlotNameがNoneでSlotNamesがEmptyのことがある。
					if (SlotNames.IsEmpty() == false)
					{
						SNPLUGIN_ASSERT(Index < SlotNames.Num(), TEXT("USNMaterialComponent::BeginPlay : Material Num is Over."));

						SlotName = SlotNames[Index];
					}
					
					Materials.Add(SlotName, MaterialInfo);
				}
			}
		}
	}
}

void USNMaterialComponent::SetScalarParameterValue(const FName& SlotName, const FName& ParamName, float Value)
{
	FSNMaterial* Material = Materials.Find(SlotName);

	if (Material != nullptr)
	{
		Material->MaterialInstanceDynamic->SetScalarParameterValue(ParamName, Value);
	}
}

void USNMaterialComponent::SetTextureParameterValue(const FName& SlotName, const FName& ParamName, UTexture* Texture)
{
	FSNMaterial* Material = Materials.Find(SlotName);

	if (Material != nullptr)
	{
		Material->MaterialInstanceDynamic->SetTextureParameterValue(ParamName, Texture);
	}
}

void USNMaterialComponent::SetRuntimeVirtualTextureParameterValue(const FName& SlotName, const FName& ParamName, URuntimeVirtualTexture* Texture)
{
	FSNMaterial* Material = Materials.Find(SlotName);

	if (Material != nullptr)
	{
		Material->MaterialInstanceDynamic->SetRuntimeVirtualTextureParameterValue(ParamName, Texture);
	}
}

void USNMaterialComponent::SetVectorParameterValue(const FName& SlotName, const FName& ParamName, const FVector& Value)
{
	FSNMaterial* Material = Materials.Find(SlotName);

	if (Material != nullptr)
	{
		Material->MaterialInstanceDynamic->SetVectorParameterValue(ParamName, Value);
	}
}


// Called when the game starts
void USNMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetComponentTickEnabled(false);
}

void USNMaterialComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AActor* Character = Cast<AActor>(GetOwner());

	if (Character != nullptr)
	{
		UMeshComponent* MeshComponent = Character->FindComponentByClass<UMeshComponent>();

		SNPLUGIN_ASSERT(MeshComponent != nullptr, TEXT("USMaterialComponent::EndPlay: Mesh is null"));

		const TArray<FName> SlotNames(MeshComponent->GetMaterialSlotNames());
		
		for (auto& Material : Materials)
		{
			int MaterialNo = SlotNames.Num() > 0 ? SlotNames.Find(Material.Key) : 0;

			if (MaterialNo >= 0)
			{
				MeshComponent->SetMaterial(MaterialNo, Material.Value.SourceInterface);	
			}
		}
	}
}

