// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SoldierBaseCharacter.h"
#include "RangedSoldierCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API ARangedSoldierCharacter : public ASoldierBaseCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Projectile")
	TSubclassOf<class AProjectileBase> Projectile;
	
public:
	virtual void Attack() override;

	void virtual AttackHitCheck() override;
};
