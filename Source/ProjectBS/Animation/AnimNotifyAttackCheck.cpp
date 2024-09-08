// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyAttackCheck.h"

#include "Interface/IAnimantionAttackInterface.h"

void UAnimNotifyAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp)
	{
		AActor* ownerActor =  MeshComp->GetOwner();

		IIAnimantionAttackInterface* attackPawn = Cast<IIAnimantionAttackInterface>(ownerActor);

		if(attackPawn != nullptr)
		{
			attackPawn->AttackHitCheck();
		}
	}
}
