// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServiceDetect.h"

#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

void UBTServiceDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Actor->GetActorLabel() == TEXT("SM_ChamferCube2"))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,Actor);

			UE_LOG(LogTemp, Warning, TEXT("@@@@Find Target : %s"), *Actor->GetName());
		}
	}
	
		
	
	// if(target != nullptr)
	// {
	// 	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET,target);
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("@@@@Find Target : %s"), *target->GetName());
	// }
}
