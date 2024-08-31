// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceDetect.h"

#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

void UBTServiceDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	if(TargetActor == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			if (Actor->GetActorLabel() == TEXT("SM_ChamferCube2"))
			{
				TargetActor = Actor;
			

				UE_LOG(LogTemp, Warning, TEXT("@@@@Find Target : %s"), *Actor->GetName());
			}
		}
	}
	
	APawn* ownerPawn =  OwnerComp.GetAIOwner()->GetPawn();

	if(ownerPawn!=nullptr)
	{
		float distance = ownerPawn->GetDistanceTo(TargetActor);

		if(distance > 200.f)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,TargetActor);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,nullptr);
		}
	}
	
}
