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

//!@{@defgroup アクション
//!@{
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
	
	//! @{@name BehaiviorTreeから呼ばれるExecuteTaskのオーバーライド
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
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
	
	//! @{@name アクションタグを取得
	FGameplayTag GetActionTag() const ;
	//! @}
	
	//! @{@name 排他用ゲームプレイタグリストを設定
	void SetExclusiveActionTag(const FGameplayTagContainer& Tags);
	void SetExclusiveActionTag(const FGameplayTag& Tag);
	//! @}
	
	//! @{@name 排他用ゲームプレイタグリストに追加
	void AddExclusiveActionTag(const FGameplayTagContainer& Tags);
	void AddExclusiveActionTag(const FGameplayTag& Tag);
	//! @}
	
	//! @{@name 排他用ゲームプレイタグリストを取得
	FGameplayTagContainer GetExclusiveActionTag() const ;
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
	
	//! @{@name BehaiviorTreeのExecTaskからの呼び出し処理
	virtual bool ExecAIAction(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){ return false; };
	//! @}
	
	//! @{@name サーバーでアクションを実行するかのフラグを設定
	void SetExecOnServer(bool bFlag);
	//! @}

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category="Action")
	bool bDebugDraw = false;
#endif
	
	//!< アクションタグ
	UPROPERTY(EditAnywhere, Category="Action")
	FGameplayTag ActionTag;
	
	//!< 排他用アクションタグリスト
	UPROPERTY(EditAnywhere, Category="Action")
	FGameplayTagContainer ExclusiveTags;
	
private:
	
	//!< サーバーでアクションを実行するかのフラグ
	UPROPERTY(EditAnywhere, Category="Online")
	bool bExecOnServer = false;
	
	//!< サーバーとクライアントの両方でアクションを実行するかのフラグ
	UPROPERTY(EditAnywhere, Category="Online")
	bool bExecOnMulticast = false;
	
	//!< オーナー
	UPROPERTY()
	TObjectPtr<UObject>		Owner = nullptr;
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
FORCEINLINE void USNActionBase::SetActionTag(const FGameplayTag& Tag){
	ActionTag = Tag;
}

//----------------------------------------------------------------------//
//
//! @brief 排他用ゲームプレイタグリストを設定
//
//! @param Tags ゲームプレイタグリスト
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::SetExclusiveActionTag(const FGameplayTagContainer& Tags){
	ExclusiveTags = Tags;
}

//----------------------------------------------------------------------//
//
//! @brief 排他用ゲームプレイタグリストを設定
//
//! @param Tags ゲームプレイタグ
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::SetExclusiveActionTag(const FGameplayTag& Tag){
	ExclusiveTags = FGameplayTagContainer(Tag);
}

//----------------------------------------------------------------------//
//
//! @brief 排他用ゲームプレイタグリストを追加
//
//! @param Tags ゲームプレイタグリスト
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::AddExclusiveActionTag(const FGameplayTagContainer& Tags){
	ExclusiveTags.AppendTags(Tags);
}

//----------------------------------------------------------------------//
//
//! @brief 排他用ゲームプレイタグリストを追加
//
//! @param Tags ゲームプレイタグ
//
//----------------------------------------------------------------------//
FORCEINLINE void USNActionBase::AddExclusiveActionTag(const FGameplayTag& Tag){
	ExclusiveTags.AddTag(Tag);
}
	
//----------------------------------------------------------------------//
//
//! @brief 排他用ゲームプレイタグリストを取得
//
//! @retval 排他用ゲームプレイタグリスト
//
//----------------------------------------------------------------------//
FORCEINLINE FGameplayTagContainer USNActionBase::GetExclusiveActionTag() const {
	return ExclusiveTags;
}

//----------------------------------------------------------------------//
//
//! @brief アクションタグを取得
//
//! @retval アクションタグ
//
//----------------------------------------------------------------------//
FORCEINLINE FGameplayTag USNActionBase::GetActionTag() const {
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
	return bExecOnMulticast;
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
//! @}
//! @}
