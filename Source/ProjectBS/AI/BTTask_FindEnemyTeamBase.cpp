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
	
	
	ETeamType ownerTeam = aiCharacter->GetTeam();

	
	if(EnemyTeamBaseActor == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor);
			if(SpawnArea!= nullptr && SpawnArea->GetTeamType() != ownerTeam)
			{
				EnemyTeamBaseActor = Actor;

			
				FString otherTeamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
				UE_LOG(LogTemp, Warning, TEXT("@@@@Find Enemy Team Spawn : %s / %s / %s"), *ownerPawn->GetName(), *Actor->GetName(), *otherTeamStr);
			}
			
		}
	}

	//적팀 베이스 찾았으면
	if(EnemyTeamBaseActor != nullptr)
	{
		FNavLocation FindEnemyTeamBaseLocation;
		// 적팀 베이스 근처 위치 찾아 블랙보드에 저장한다.
		if(navSystem->GetRandomPointInNavigableRadius(EnemyTeamBaseActor->GetActorLocation(),300.f,FindEnemyTeamBaseLocation))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ENEMYSPAWN_LOCATION,FindEnemyTeamBaseLocation);

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
