// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToCustom.generated.h"

USTRUCT()
struct FBTTask_MoveToCustomMemory
{
	GENERATED_BODY()

	UBehaviorTreeComponent* CachedOwnerComp;
};

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBTTask_MoveToCustom : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToCustom();
	
protected:

virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;

	// virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//
	//
	// virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

private:

	// 이동 완료 시 호출되는 델리게이트 함수입니다.
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

};
