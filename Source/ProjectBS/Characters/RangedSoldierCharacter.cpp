// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RangedSoldierCharacter.h"

#include "Projectile/ProjectileBase.h"

void ARangedSoldierCharacter::Attack()
{
	Super::Attack();
}

void ARangedSoldierCharacter::AttackHitCheck()
{
	// Super::AttackHitCheck();


	AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, HitPointStart->GetComponentLocation(), GetActorRotation());
	// if (projectile != nullptr)
	
}
