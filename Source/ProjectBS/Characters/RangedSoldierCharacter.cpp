// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RangedSoldierCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Projectile/ProjectileBase.h"

void ARangedSoldierCharacter::Attack()
{
	Super::Attack();
}

void ARangedSoldierCharacter::AttackHitCheck()
{
	// Super::AttackHitCheck();
	ASoldierBaseCharacter* character = Cast<ASoldierBaseCharacter>(this);
	if(character)
	{
		FVector location = HitPointStart->GetComponentLocation();
		FRotator rotation = GetActorRotation();
	
		AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, location, rotation);

		projectile->SetInstigator(character);

		projectile->SetDamage(GetStatComponent()->GetAttackDamange());
	
		ETeamType teamType = GetTeam();
		projectile->SetCollisionProfileName(teamType);
	}
}
