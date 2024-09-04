// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceDetect.h"

#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	

	if(TargetActor == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor);
			if(SpawnArea!= nullptr && SpawnArea->GetTeamType() == ownerTeam)
			{
				TargetActor = Actor;

				FString ownerTeamStr = StaticEnum<ETeamType>()->GetValueAsString(ownerTeam);
				UE_LOG(LogTemp, Warning, TEXT("@@@@Find Owen Spawn : %s / %s"), *Actor->GetName(), *ownerTeamStr);

			}
			
		}
	}
	

	if(ownerPawn!=nullptr)
	{
		float distance = ownerPawn->GetDistanceTo(TargetActor);

		if(distance < DectectDistance)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,TargetActor);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,nullptr);
		}
	}
	
}
