// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierBaseCharacter.h"

#include "AI/SoldierAIController.h"

// Sets default values
ASoldierBaseCharacter::ASoldierBaseCharacter()
{

	AIControllerClass = ASoldierAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Characters/Mannequins/Animations/AM_Attack.AM_Attack'"));
	if(AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}
}

// Called when the game starts or when spawned
void ASoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoldierBaseCharacter::Attack()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance != nullptr)
	{
		animInstance->Montage_Play(AttackMontage);
	}
}





