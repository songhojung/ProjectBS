// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindOtherTeamBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBTTask_FindOtherTeamBase : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:
	AActor* EnemyActor;
};