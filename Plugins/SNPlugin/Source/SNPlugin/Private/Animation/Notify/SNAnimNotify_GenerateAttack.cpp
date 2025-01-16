// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SNAnimNotify_GenerateAttack.h"

#include "Character/Components/SNAttackComponent.h"

void USNAnimNotify_GenerateAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FVector StartPos = (bSweep == true) ? Start : Position;
	FVector EndPos =   (bSweep == true) ? End : Position;
	
	if (ParentSocket != NAME_None)
	{
		FTransform Transform(MeshComp->GetSocketTransform(ParentSocket));

		if (bIgnoreRotation == true)
		{
			AActor* OwnerActor = Cast<AActor>(MeshComp->GetOwner());
			
			FRotator Rotator(OwnerActor->GetActorRotation());

			Transform.SetRotation(Rotator.Quaternion());
		}
		
		StartPos = Transform.TransformPosition(StartPos);
		EndPos = Transform.TransformPosition(EndPos);
	}

	AActor* Attacker = Cast<AActor>(MeshComp->GetOwner());

	if (Attacker != nullptr)
	{
		USNAttackComponent* AttackComponent = Attacker->FindComponentByClass<USNAttackComponent>();

		if (AttackComponent != nullptr)
		{
			AttackComponent->GenerateSphereSweep(Attacker, Time, StartPos, EndPos, Radius, DamageAttribetes, TArray<AActor*>(), bPenetrate, bBomb);
		}
	}
}
