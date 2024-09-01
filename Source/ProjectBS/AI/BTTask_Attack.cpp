// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Interface/SoldierCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	APawn* ContorllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ContorllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ContorllingPawn);
	if(aiCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	aiCharacter->Attack();

	return EBTNodeResult::Succeeded;

}
