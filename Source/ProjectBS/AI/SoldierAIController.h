// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoldierAIController.generated.h"

#define BBKEY_TARGET TEXT("Target")
#define BBKEY_ENEMYSPAWN_LOCATION TEXT("EnemySpawnLocation")

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

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
};
