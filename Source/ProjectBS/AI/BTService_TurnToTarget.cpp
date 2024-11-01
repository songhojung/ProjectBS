// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_TurnToTarget.h"

#include "AIController.h"
#include "SoldierAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/SoldierCharacterAIInterface.h"


void UBTService_TurnToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	float turnSpeed = aiCharacter->GetAITurnSpeed();
	FVector dir = targetPawn->GetActorLocation() - ownerPawn->GetActorLocation();
	dir.Z = 0.f;
	
	FRotator targetRot = FRotationMatrix::MakeFromX(dir).Rotator();
	ownerPawn->SetActorRotation(FMath::RInterpTo(ownerPawn->GetControlRotation(),targetRot,GetWorld()->GetDeltaSeconds(),turnSpeed));
}
