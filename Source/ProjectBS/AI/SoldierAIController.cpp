// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SoldierAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ASoldierAIController::ASoldierAIController()
{
	static  ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ProjectBS/AI/BB_SoldierCharacter.BB_SoldierCharacter'"));
	if(BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static  ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ProjectBS/AI/BT_SoldierCharacter.BT_SoldierCharacter'"));
	if(BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIBlackboardComponent"));
}

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BBAsset && BTAsset)
	{
		if(UseBlackboard(BBAsset,BlackboardComponent))
		{
			RunBehaviorTree(BTAsset);
		}
	}
}

void ASoldierAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		// BrainComponent->Cleanup();
		BTComponent->StopTree();
	}
}

