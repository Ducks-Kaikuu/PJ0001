// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotifyState_GenerateAttack.h"
#include "Character/Components/SNAttackComponent.h"

void USNAnimNotifyState_GenerateAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FVector StartPos = (bSweep == true) ? Start : Position;
	FVector EndPos =   (bSweep == true) ? End : Position;
	
	if (ParentSocket != NAME_None)
	{
		FTransform Transform(MeshComp->GetSocketTransform(ParentSocket));

		StartPos = Transform.TransformPosition(StartPos);
		EndPos = Transform.TransformPosition(EndPos);
	}

	AActor* Attacker = Cast<AActor>(MeshComp->GetOwner());

	if (Attacker != nullptr)
	{
		USNAttackComponent* AttackComponent = Attacker->FindComponentByClass<USNAttackComponent>();

		if (AttackComponent != nullptr)
		{
			TArray<AActor*> IgnoreList({Attacker});
			
			AttackComponent->GenerateSphereSweep(Attacker, Time, StartPos, EndPos, Radius, DamageAttribetes, IgnoreList, bPenetrate, bBomb);
		}
	}
}
