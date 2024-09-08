// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierBaseCharacter.h"

#include "AI/SoldierAIController.h"
#include "CharacterProperty/SoldierStatComponent.h"
#include "CharacterProperty/TeamComponent.h"

// Sets default values
ASoldierBaseCharacter::ASoldierBaseCharacter()
{

	AIControllerClass = ASoldierAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectBS/Animation/AM_Attack.AM_Attack'"));
	if(AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}


	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("SoldierTeamComponent"));

	StatComponent = CreateDefaultSubobject<USoldierStatComponent>(TEXT("SoldierStatsComponent"));
}

// Called when the game starts or when spawned
void ASoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();


	MeshComponent = GetMesh();



}




void ASoldierBaseCharacter::SetTeam(ETeamType Team)
{
	if(TeamComponent == nullptr)
		return;;

	TeamComponent->SetTeamType(Team);
	TeamComponent->SetTeamDynamicMaterialInstanceOverride(GetMesh(),Team);
	
}

ETeamType ASoldierBaseCharacter::GetTeam() const
{
	return TeamComponent->GetTeamType();
}

AActor& ASoldierBaseCharacter::GetOtherTeamBaseActor() const
{
	return  TeamComponent->GetOtherTeamBaseActor();
}

float ASoldierBaseCharacter::GetAIDectectRange()
{
	return StatComponent->GetDetectRange();
}


void ASoldierBaseCharacter::Attack()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance != nullptr)
	{
		animInstance->Montage_Play(AttackMontage);
	}
}





