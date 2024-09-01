// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SoldierCharacterAIInterface.h"
#include "SoldierBaseCharacter.generated.h"

UCLASS()
class PROJECTBS_API ASoldierBaseCharacter : public ACharacter ,public ISoldierCharacterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldierBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//AttackSection
public:
	virtual void Attack() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	
public:	

	

};
