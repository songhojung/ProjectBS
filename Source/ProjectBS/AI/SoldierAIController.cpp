// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SoldierAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SoldierBaseCharacter.h"

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

	
}

// void ASoldierAIController::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
// 	UE_LOG(LogTemp, Error, TEXT("ASoldierAIController() OnPossess %s"),*this->GetName());
//
//
// }

void ASoldierAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		// BrainComponent->Cleanup();
		BTComponent->StopTree();
	}
}

void ASoldierAIController::StartAI()
{
	// 확인: Blackboard 및 BehaviorTree가 유효한지 체크
	if (!BBAsset || !BTAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Blackboard or Behavior Tree asset is not set in SoldierAIController"));
		return;
	}
	
	if(BBAsset && BTAsset)
	{
		if(UseBlackboard(BBAsset,BlackboardComponent))
		{
			RunBehaviorTree(BTAsset);
		}
	}
	
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BTComponent)
	{
		ASoldierBaseCharacter* soldier = Cast<ASoldierBaseCharacter>(GetPawn()) ;
		if(soldier)
		{
			float length = soldier->GetAttackAnimLength();
			BTComponent->GetBlackboardComponent()->SetValueAsFloat(BBKEY_ATK_ANIM_LENGTH,length);
		}
	}
}

