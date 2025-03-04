// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotify_RemoveActionTag.h"
#include "SNDef.h"
#include "Character/Base/SNCharacterBase.h"

//----------------------------------------------------------------------//
//
//! @brief ノーティファイ
//
//! @param MeshComp       メッシュコンポーネント
//! @param Animation      再生中のアニメーション
//! @param EventReference イベント情報
//
//----------------------------------------------------------------------//
void USNAnimNotify_RemoveActionTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference){
	
	Super::Notify(MeshComp, Animation, EventReference);
	
	ASNCharacterBase* Character(Cast<ASNCharacterBase>(MeshComp->GetOwner()));
	
	if(Character != nullptr){
		// アクションタグを削除
		Character->RemoveActionTagContainer(Tags);
#if WITH_EDITORONLY_DATA
		//if(bDebugDraw == true){
			SNPLUGIN_LOG(TEXT("Remove Action Tag Container.[%s] - [%s]"), *Tags.ToString(), *Animation->GetName());
		//}
#endif
	}
}
