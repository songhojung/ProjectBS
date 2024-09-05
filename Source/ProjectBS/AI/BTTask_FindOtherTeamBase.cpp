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

		UWorld* world = ownerPawn->GetWorld();
		
		// 결과를 저장할 배열
		TArray<FOverlapResult> OverlapResults;

		//위치
		FVector center = ownerPawn->GetActorLocation();
		
		// 충돌 모양 정의 (구)
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(500.f);  // 반경 설정
		
		// 충돌 쿼리 파라미터 (필요한 경우)
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ownerPawn);  // 자신의 액터는 무시
		
		// 콜리전 채널 설정 (예: ECC_Pawn)
		ECollisionChannel CollisionChannel = ECC_Pawn;
		
		// OverlapMultiByChannel 호출
		bool bHasOverlaps = world->OverlapMultiByChannel(
			OverlapResults,            // 결과 배열
			center            // 검색 위치
			FQuat::Identity,           // 회전 (기본적으로 회전 없음)
			CollisionChannel,          // 콜리전 채널
			CollisionShape,            // 콜리전 모양 (구)
			QueryParams                // 쿼리 파라미터
		);

		if(bHasOverlaps)
		{
			
		}

		DrawDebugSphere(world, center, DetectRadius, 16, FColor::Red, false, 0.2f);

	}

	return EBTNodeResult::Succeeded;
}
