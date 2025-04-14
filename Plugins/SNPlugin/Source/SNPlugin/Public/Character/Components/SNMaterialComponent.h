// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SNMaterialComponent.generated.h"

//!@{@defgroup マテリアル
//!@{
//----------------------------------------------------------------------//
//
//! @brief マテリアル構造体
//
//----------------------------------------------------------------------//
USTRUCT(BlueprintType)
struct FSNMaterial
{
	GENERATED_BODY()
	
	//! @{@name デフォルトコンストラクタ
	FSNMaterial():SourceInterface(nullptr), MaterialInstanceDynamic(nullptr){}
	
	FSNMaterial(UMaterialInterface* Source, UMaterialInstanceDynamic* Destination):
	SourceInterface(Source),
	MaterialInstanceDynamic(Destination)
	{}
	//! @}
	
	//!< 元となるマテリアルインターフェイス
	UPROPERTY()
	TObjectPtr<UMaterialInterface> SourceInterface = nullptr;
	
	//!< マテリアルダイナミックインスタンス
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic = nullptr;
};
//! @}
//! @}

//!@{@defgroup マテリアル
//!@{
//----------------------------------------------------------------------//
//
//! @brief マテリアル操作クラス
//
//----------------------------------------------------------------------//
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SNPLUGIN_API USNMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//! @{@name デフォルトコンストラクタ
	USNMaterialComponent();
	//! @}
	
	//! @{@name ダイナミックインスタンスを生成
	void CreateMaterialInstanceDynamic();
	//! @}
	
	//! @{@name Float型のパラメータの値を設定
	void SetScalarParameterValue(const FName& SlotName, const FName& ParamName, float Value);
	//! @}
	
	
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
//! @}
//! @}
