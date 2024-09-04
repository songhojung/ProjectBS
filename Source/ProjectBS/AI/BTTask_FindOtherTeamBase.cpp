// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindOtherTeamBase.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

EBTNodeResult::Type UBTTask_FindOtherTeamBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn==nullptr)
		return EBTNodeResult::Failed;

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ownerPawn);
	if(aiCharacter == nullptr)
		return EBTNodeResult::Failed;

	ETeamType ownerTeam = aiCharacter->GetTeam();
	

	if(EnemyActor == nullptr)
	{
		// TArray<AActor*> FoundActors;
		// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundActors);
		// for (AActor* Actor : FoundActors)
		// {
		// 	ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor);
		// 	if(SpawnArea!= nullptr && SpawnArea->GetTeamType() == ownerTeam)
		// 	{
		// 		EnemyActor = Actor;
		//
		// 		FString ownerTeamStr = StaticEnum<ETeamType>()->GetValueAsString(ownerTeam);
		// 		UE_LOG(LogTemp, Warning, TEXT("@@@@Find Owen Spawn : %s / %s"), *Actor->GetName(), *ownerTeamStr);
		//
		// 	}
		// 	
		// }


		
		// TArray<FOverlapResult> OverlapResults;
		// FCollisionQueryParams CollisionQueryParm(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
		// bool bResult = world->OverlapMultiByChannel(
		// 	OverlapResults,
		// 	center,
		// 	FQuat::Identity,
		// 	CCHANNEL_ABACTION,
		// 	FCollisionShape::MakeSphere(DetectRadius),
		// 	CollisionQueryParm
		// );
	}

	return EBTNodeResult::Succeeded;
}
