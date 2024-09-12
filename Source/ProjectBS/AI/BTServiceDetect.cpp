// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceDetect.h"

#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

void UBTServiceDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn==nullptr)
		return;

	ISoldierCharacterAIInterface* aiCharacter = Cast<ISoldierCharacterAIInterface>(ownerPawn);
	if(aiCharacter == nullptr)
		return;

	ETeamType ownerTeam = aiCharacter->GetTeam();
	float detectRange = aiCharacter->GetAIDectectRange();


	UWorld* world = ownerPawn->GetWorld();
		
	// 결과를 저장할 배열
	TArray<FOverlapResult> OverlapResults;
		
	//위치
	FVector center = ownerPawn->GetActorLocation();
		
	// 충돌 모양 정의 (구)
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(detectRange);  // 반경 설정
		
	// 충돌 쿼리 파라미터 (필요한 경우)
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ownerPawn);  // 자신의 액터는 무시
		
	// 콜리전 채널 설정 (예: ECC_Pawn)
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel1; //SoldierPawn 채널
		
	// OverlapMultiByChannel 호출
	bool bHasOverlaps = world->OverlapMultiByChannel(
		OverlapResults,            // 결과 배열
		center,            // 검색 위치
		FQuat::Identity,           // 회전 (기본적으로 회전 없음)
		CollisionChannel,          // 콜리전 채널
		CollisionShape,            // 콜리전 모양 (구)
		QueryParams                // 쿼리 파라미터
	);

	AActor* targetActor = nullptr;
	float nearestDistance = 0.f;
	if(bHasOverlaps)
	{
		for (auto const& overlapResult : OverlapResults)
		{
			AActor* overLapActor = overlapResult.GetActor();
				
			ISoldierCharacterAIInterface* soliderPawn = Cast<ISoldierCharacterAIInterface>(overLapActor);

			//다른팀이고 죽지 않았다면
			if(soliderPawn != nullptr && soliderPawn->GetTeam() != ownerTeam && soliderPawn->IsDead() ==false)
			{

				float distance = ownerPawn->GetDistanceTo(overLapActor);
				//제일 가까운 적으로 타켓으로 삼는다
				if(nearestDistance <= 0.f ||  nearestDistance > distance)
				{
					targetActor = overLapActor;
					// DrawDebugSphere(world, center, detectRange, 16, FColor::Red, false, 0.2f);
				}
			}
		}
	}

	// DrawDebugSphere(world, center, detectRange, 16, FColor::Green, false, 0.2f);

	// if(targetActor == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("**** dect target is null : %s"), *ownerPawn->GetName() );
	//
	// }
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,targetActor);
}
