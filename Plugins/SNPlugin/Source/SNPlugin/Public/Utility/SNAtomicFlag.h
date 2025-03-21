// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNDef.h"

//----------------------------------------------------------------------//
//
//! @brief スレッドセーフなフラグ管理クラス
//
//----------------------------------------------------------------------//
template<int SIZE>
class SNAtomicFlag
{
public:
	//! @{@name デフォルトコンストラクタ
	SNAtomicFlag();
	//! @}

	//! @{@name デストラクタ
	~SNAtomicFlag();
	//! @}

	//! @{@name フラグをクリアします
	void Clear();
	//! @}

	//! @{@name 指定された番号のフラグを立てます
	void On(int No);
	//! @}

	//! @{@name 指定された番号のフラグを寝かせます
	void Off(int No);
	//! @}

	//! @{@name 指定された番号のフラグが立っているかチェックします
	bool Is(int No) const ;
	//! @}

private:
	std::atomic<uint32_t> Flag[SIZE];
};

//----------------------------------------------------------------------//
//
//! @brief デフォルトコンストラクタ
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE SNAtomicFlag<SIZE>::SNAtomicFlag()
{
	
}

//----------------------------------------------------------------------//
//
//! @brief デストラクタ
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE SNAtomicFlag<SIZE>::~SNAtomicFlag()
{
	
}

//----------------------------------------------------------------------//
//
//! @brief フラグをクリアします
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE void SNAtomicFlag<SIZE>::Clear(){
	for (int i = 0; i < SIZE; i++)
	{
		Flag[i].store(0);
	}
}

//----------------------------------------------------------------------//
//
//! @brief 指定された番号のフラグを立てます
//
//! @param No 番号
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE void SNAtomicFlag<SIZE>::On(int No)
{
	int Index=(No/(sizeof(uint32_t)*8));
	int BitNo=(No%(sizeof(uint32_t)*8));

	SNPLUGIN_ASSERT(Index < SIZE            , TEXT("Flag Index Is Over\n"));
	SNPLUGIN_ASSERT(BitNo < sizeof(uint32)*8, TEXT("Flag Index Is Over\n"));

	uint32_t Value=Flag[Index].load();

	Value |=(0x00000001 << BitNo);
	
	Flag[Index].store(Value);
}

//----------------------------------------------------------------------//
//
//! @brief 指定された番号のフラグを寝かせます
//
//! @param No 番号
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE void SNAtomicFlag<SIZE>::Off(int No)
{
	int Index=(No/(sizeof(uint32_t)*8));
	int BitNo=(No%(sizeof(uint32_t)*8));

	SNPLUGIN_ASSERT(Index < SIZE            , TEXT("Flag Index Is Over\n"));
	SNPLUGIN_ASSERT(BitNo < sizeof(uint32)*8, TEXT("Flag Index Is Over\n"));

	uint32_t Value=Flag[Index].load();

	Value &=~(0x00000001 << BitNo);
	
	Flag[Index].store(Value);
}

//----------------------------------------------------------------------//
//
//! @brief 指定された番号のフラグが立っているかチェックします
//
//! @param No 番号
//
//! @retval true  フラグが立っている
//! @retval false フラグが立っていない
//
//----------------------------------------------------------------------//
template<int SIZE>
FORCEINLINE bool SNAtomicFlag<SIZE>::Is(int No) const
{
	int Index=(No/(sizeof(uint32_t)*8));
	int BitNo=(No%(sizeof(uint32_t)*8));

	SNPLUGIN_ASSERT(Index < SIZE            , TEXT("Flag Index Is Over\n"));
	SNPLUGIN_ASSERT(BitNo < sizeof(uint32)*8, TEXT("Flag Index Is Over\n"));
	
	uint32_t Value=Flag[Index].load();

	return Value & (0x00000001 << BitNo) ? true : false;
}
