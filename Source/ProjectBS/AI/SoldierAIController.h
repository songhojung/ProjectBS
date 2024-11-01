// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoldierAIController.generated.h"

#define BBKEY_TARGET TEXT("Target")
#define BBKEY_TARGET_LOCATION TEXT("TargetLocation")
#define BBKEY_ENEMYSPAWN_LOCATION TEXT("EnemySpawnLocation")
#define BBKEY_ATK_ANIM_LENGTH TEXT("AtkAnimLength")
#define BBKEY_IS_IN_ATTACK_RANGE TEXT("IsInAttackRange")	

/**
 * 
 */
UCLASS()
class PROJECTBS_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()
public:
  ASoldierAIController();

protected:
	virtual void BeginPlay() override;
	// virtual void OnPossess(APawn* InPawn) override;
protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

public:
	void StopAI();
	void StartAI();
	void DetectedTargetClear();
};
