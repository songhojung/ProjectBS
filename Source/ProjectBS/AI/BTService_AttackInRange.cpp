// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_AttackInRange.h"

#include "AIController.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/SoldierCharacterAIInterface.h"

void UBTService_AttackInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn==nullptr)
		return;

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ownerPawn);
	if(aiCharacter == nullptr)
		return;

	UObject* targetObj =  OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);

	APawn* targetPawn = Cast<APawn>(targetObj);
	if(targetPawn==nullptr)
		return;

	float attackRange = aiCharacter->GetAIAttackRange();
	
#if WITH_EDITOR
	DrawDebugSphere(ownerPawn->GetWorld(), ownerPawn->GetActorLocation(), attackRange /*/ 2*/, 16, FColor::Blue, false, 0.2f);
#endif
	float distance =ownerPawn->GetDistanceTo(targetPawn);

	//거리안이면 true반환
	bool isInAttackRange = attackRange >= distance;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_ATTACK_RANGE,isInAttackRange);
}
