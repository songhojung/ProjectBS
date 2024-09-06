// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindEnemyTeamBase.h"

#include "NavigationSystem.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

EBTNodeResult::Type UBTTask_FindEnemyTeamBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn==nullptr)
		return EBTNodeResult::Failed;

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ownerPawn);
	if(aiCharacter == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(ownerPawn->GetWorld());
	if(navSystem==nullptr)
		return EBTNodeResult::Failed;
	
	
	// ETeamType ownerTeam = aiCharacter->GetTeam();

	AActor& otherTeamBaseActor = aiCharacter->GetOtherTeamBaseActor();

	FNavLocation FindEnemyTeamBaseLocation;
	// 적팀 베이스 근처 위치 찾아 블랙보드에 저장한다.
	if(navSystem->GetRandomPointInNavigableRadius(otherTeamBaseActor.GetActorLocation(),300.f,FindEnemyTeamBaseLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ENEMYSPAWN_LOCATION,FindEnemyTeamBaseLocation);

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
