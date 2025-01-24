// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "InputActionValue.h"
#include "SNActionBase.generated.h"

class USNInputConfig;
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
class UEnhancedInputComponent;
class UInputAction;
class AActor;

struct FInputActionValue;

//----------------------------------------------------------------------//
//
//! @brief 入力されたパラメータへアクセスするためのプロキシ
//
//----------------------------------------------------------------------//
USTRUCT()
struct FInputActionValueProxy:public FInputActionValue
{
	GENERATED_BODY()
	
	//! @{@name デフォルトコンストラクタ
	FInputActionValueProxy();
	//! @}
	
	//! @{@name コピーコンストラクタ
	FInputActionValueProxy(const FInputActionValue& InputActionValue);
	//! @}
	
	//! @{@name 入力値を取得
	const FVector& GetValue() const ;
	//! @}
	
	//! @{@name 入力タイプを取得
	EInputActionValueType GetType() const ;
	//! @}
};

//----------------------------------------------------------------------//
//
//! @brief 入力値を取得
//
//! @retval 入力値
//
//----------------------------------------------------------------------//
FORCEINLINE const FVector& FInputActionValueProxy::GetValue() const {
	return Value;
}

//----------------------------------------------------------------------//
//
//! @brief 入力タイプを取得
//
//! @retval 入力タイプ
//
//----------------------------------------------------------------------//
FORCEINLINE EInputActionValueType FInputActionValueProxy::GetType() const {
	return ValueType;
}

//----------------------------------------------------------------------//
//
//! @brief アクション用ベースクラス
//
//----------------------------------------------------------------------//
UCLASS(BlueprintType, Blueprintable)
class SNPLUGIN_API USNActionBase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// ISNPlayablePawnInterfaceのみExecActionを呼び出せるように。
	friend class ISNPlayablePawnInterface;
	
	//! @{@name デフォルトコンストラクタ
	USNActionBase();
	//! @}
	
	//! @{@name デストラクタ
	virtual ~USNActionBase();
	//! @}
	
	//! @{@name 初期化処理
	virtual void Initialize(UEnhancedInputComponent* InputComponent, const UInputAction* InputAction, UObject* Object);
	//! @}
	
	//! @{@name オーナーを設定
	void	SetOwner(AActor* Object) override ;
	//! @}
	
	//! @{@name 入力からのアクション
	void	InputAction(const FInputActionValue& InputActionValue);
	//! @}
	
	//! @{@name オーナーを取得
	template<class T>
	T* GetOwner();
	
	template<class T>
	const T* GetOwner() const ;
	//! @}
	
	//! @{@name アクションタグの設定
	void SetActionTag(const FGameplayTag& Tag);
	//! @}

	void SetExclusiveActionTag(const FGameplayTagContainer& Tags);
	
	//! @{@name アクションタグを取得
	FGameplayTag GetActionTag() const ;
	//! @}
	
	//! @{@name サーバーとクライアント両方で実行するかチェック
	bool IsExecOnBoth() const ;
	//! @}
	
	//! @{@name サーバーのみで実行するかチェック
	bool IsExecOnServer() const ;
	//! @}
	
protected:
	
	//! @{@name 各アクションの実行処理
	virtual bool ExecAction(const FInputActionValue& InputActionValue){ return false; };
	//! @}
	
	//! @{@name サーバーでアクションを実行するかのフラグを設定
	void SetExecOnServer(bool bFlag);
	//! @}
	
private:
	
	//!< サーバーでアクションを実行するかのフラグ
	UPROPERTY(EditAnywhere, Category="Online")
	bool bExecOnServer = false;
	
	//!< サーバーとクライアントの両方でアクションを実行するかのフラグ
	UPROPERTY(EditAnywhere, Category="Online")
	bool bExecOnBoth = false;
	
	//!< アクション名
	FName ActionName;

	UPROPERTY()
	FGameplayTag ActionTag;

	UPROPERTY()
	FGameplayTagContainer ExclusiveTags;
	
	//!< オーナー
	UPROPERTY()
	UObject*	Owner;
};

//----------------------------------------------------------------------//
//
//! @brief オーナーを設定
//
//! @param Object オーナーとなるオブジェクト
//
//----------------------------------------------------------------------//
FORCEINLINE void	USNActionBase::SetOwner(AActor* Object){
	
	Super::SetOwner(Object);
	
	Owner = Object;
}

//----------------------------------------------------------------------//
//
//! @brief アクションタグの設定
//
//! @param Tag アクションタグ
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::SetActionTag(const FGameplayTag& Tag)
{
	ActionTag = Tag;
}

FORCEINLINE void USNActionBase::SetExclusiveActionTag(const FGameplayTagContainer& Tags)
{
	ExclusiveTags = Tags;
}

//----------------------------------------------------------------------//
//
//! @brief アクションタグを取得
//
//! @retval アクションタグ
//
//----------------------------------------------------------------------//
FORCEINLINE FGameplayTag USNActionBase::GetActionTag() const
{
	return ActionTag;
}
//----------------------------------------------------------------------//
//
//! @brief サーバーでアクションを実行するかのフラグを設定
//
//! @param bFlag true : サーバーで実行 / false : ローカルで実行
//
//! @retval 
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::SetExecOnServer(bool bFlag){
	bExecOnServer = bFlag;
}

//----------------------------------------------------------------------//
//
//! @brief オーナーを取得
//
//! @retval オーナーへのポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE T* USNActionBase::GetOwner(){
	return Cast<T>(Owner);
}

//----------------------------------------------------------------------//
//
//! @brief オーナーを取得
//
//! @retval オーナーへのポインタ
//
//----------------------------------------------------------------------//
template<class T>
FORCEINLINE const T* USNActionBase::GetOwner() const {
	return Cast<T>(Owner);
}

//----------------------------------------------------------------------//
//
//! @brief サーバーとクライアント両方で実行するかチェック
//
//! @retval true : マルチキャストで実行 / false : ローカルで実行
//
//----------------------------------------------------------------------//
FORCEINLINE bool USNActionBase::IsExecOnBoth() const {
	return bExecOnBoth;
}

//----------------------------------------------------------------------//
//
//! @brief サーバーのみで実行するかチェック
//
//! @retval true : サーバーで実行 / false : ローカルで実行
//
//----------------------------------------------------------------------//
FORCEINLINE bool USNActionBase::IsExecOnServer() const {
	return bExecOnServer;
}
