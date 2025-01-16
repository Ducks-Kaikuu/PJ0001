// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SNSoundHandle.h"

const FSNSoundHandle FSNSoundHandle::Invalid(0xffffffff);

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
FSNSoundHandle::FSNSoundHandle():Handle(Invalid){
	
}

//----------------------------------------------------------------------//
//
//! @brief コピーコンストラクタ
//
//! @param other 代入元
//
//----------------------------------------------------------------------//
FSNSoundHandle::FSNSoundHandle(const FSNSoundHandle& other):Handle(other.Handle){
	
}

//----------------------------------------------------------------------//
//
//! @brief コピーコンストラクタ
//
//! @param handle 代入元
//
//----------------------------------------------------------------------//
FSNSoundHandle::FSNSoundHandle(uint32 handle):Handle(handle){
	
}

//----------------------------------------------------------------------//
//
//! @brief uint32への暗黙的変換
//
//! @retval uint32型の値
//
//----------------------------------------------------------------------//
FSNSoundHandle::operator uint32() const {
	return Handle;
}

//----------------------------------------------------------------------//
//
//! @brief 代入演算子
//
//! @param rhs 代入元
//
//! @retval 自分への参照
//
//----------------------------------------------------------------------//
FSNSoundHandle& FSNSoundHandle::operator=(uint32 rhs){
	
	Handle = rhs;
	
	return *this;
}

//----------------------------------------------------------------------//
//
//! @brief 代入演算子
//
//! @param other 代入元
//
//! @retval 自分への参照
//
//----------------------------------------------------------------------//
FSNSoundHandle& FSNSoundHandle::operator=(const FSNSoundHandle& other){
	
	if(this != &other){
		Handle = other.Handle;
	}
	
	return *this;
}

//----------------------------------------------------------------------//
//
//! @brief 等式比較演算子
//
//! @param other 比較元
//
//! @retval true  等しい
//! @retval false 等しくない
//
//----------------------------------------------------------------------//
bool FSNSoundHandle::operator==(const FSNSoundHandle& other) const {
	return Handle == other.Handle;
}

//----------------------------------------------------------------------//
//
//! @brief 等式比較演算子
//
//! @param rhs 比較元
//
//! @retval true  等しい
//! @retval false 等しくない
//
//----------------------------------------------------------------------//
bool FSNSoundHandle::operator==(uint32 rhs) const {
	return Handle == rhs;
}

//----------------------------------------------------------------------//
//
//! @brief 非等式比較演算子
//
//! @param other 比較元
//
//! @retval true  等しい
//! @retval false 等しくない
//
//----------------------------------------------------------------------//
bool FSNSoundHandle::operator!=(const FSNSoundHandle& other) const {
	return Handle != other.Handle;
}

//----------------------------------------------------------------------//
//
//! @brief 非等式比較演算子
//
//! @param rhs 比較元
//
//! @retval true  等しい
//! @retval false 等しくない
//
//----------------------------------------------------------------------//
bool FSNSoundHandle::operator!=(uint32 rhs) const {
	return Handle != rhs;
}

//----------------------------------------------------------------------//
//
//! @brief ハンドルが有効かチェック
//
//! @retval true  有効
//! @retval false 無効
//
//----------------------------------------------------------------------//
bool FSNSoundHandle::IsValid() const {
	return Handle != Invalid;
}

//----------------------------------------------------------------------//
//
//! @brief ユニークIDを発行
//
//! @retval true  有効
//
//----------------------------------------------------------------------//
FSNSoundHandle FSNSoundHandle::GenerateSoundHandle(){
	return FSNSoundHandle(GEngine->GetUniqueID());
}
