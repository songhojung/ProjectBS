// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"

#include "AIController.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/SoldierCharacterAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result =  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	result = false;
	
	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn==nullptr)
		return false;

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ownerPawn);
	if(aiCharacter == nullptr)
		return false;

	UObject* targetObj =  OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);

	APawn* targetPawn = Cast<APawn>(targetObj);
	if(targetPawn==nullptr)
		return false;

	DrawDebugSphere(ownerPawn->GetWorld(), ownerPawn->GetActorLocation(), aiCharacter->GetAIAttackRange() / 2, 16, FColor::Blue, false, 0.2f);
	
	float distance =ownerPawn->GetDistanceTo(targetPawn);

	//거리안이면 true반환
	if(aiCharacter->GetAIAttackRange() >= distance)
		return true;


	return  result;
}
