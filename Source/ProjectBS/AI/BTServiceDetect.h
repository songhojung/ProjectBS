// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBTServiceDetect : public UBTService
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	AActor* TargetActor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Dectect")
	float DectectDistance = 200.f;
};
