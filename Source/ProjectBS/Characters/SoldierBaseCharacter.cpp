// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierBaseCharacter.h"

#include "AI/SoldierAIController.h"
#include "CharacterProperty/SoldierStatComponent.h"
#include "CharacterProperty/TeamComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

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

void ASoldierBaseCharacter::SetStat(float hp, float attackDamage)
{
	if(StatComponent == nullptr)
		return;

	StatComponent->SetStat(hp,attackDamage);
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


//공격하기
void ASoldierBaseCharacter::Attack()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance != nullptr)
	{
		animInstance->Montage_Play(AttackMontage);
	}
}


//공격시 히트 체크
void ASoldierBaseCharacter::AttackHitCheck()
{
	FHitResult outHitResult;
	FCollisionQueryParams params(SCENE_QUERY_STAT(Attack), false, this);
	float attackRange = 20.f;
	float attackRadius = 30.f;
	FVector start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector end  =start + GetActorForwardVector() * attackRange;
	
	bool hitDetected = GetWorld()->SweepSingleByChannel(outHitResult,start,end,FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(attackRadius),params);
	if(hitDetected)
	{
		UE_LOG(LogTemp, Warning, TEXT("hitDetected : %s"), *outHitResult.GetActor()->GetName() );

		FDamageEvent damageEvent;
		outHitResult.GetActor()->TakeDamage(StatComponent->GetAttackDamange(), damageEvent,GetController(),this);
	}

#if ENABLE_DRAW_DEBUG
	FVector capsuleOrigin = start + (end - start) * 0.5f;
	float capsuleHalfHeight = attackRange * 0.5f;
	FColor drawColor = hitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), capsuleOrigin, capsuleHalfHeight, attackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), drawColor,false, 3.0f);

#endif
}


//데미지 맞을때
float ASoldierBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComponent->ApplyDamage(DamageAmount);

	return DamageAmount;
}





