// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToCustom.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"


UBTTask_MoveToCustom::UBTTask_MoveToCustom()
{
}

void UBTTask_MoveToCustom::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);


}

EBTNodeResult::Type UBTTask_MoveToCustom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 블랙보드와 AI 컨트롤러 가져오기
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (BlackboardComp == nullptr || AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 목표 액터를 블랙보드에서 가져오기
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("Target"));
	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 자신의 캐릭터와 목표 액터의 콜라이더 반경 계산
	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	

	float SelfRadius, SelfHeight;
	ControlledPawn->GetSimpleCollisionCylinder(SelfRadius, SelfHeight);

	float TargetRadius, TargetHeight;
	TargetActor->GetSimpleCollisionCylinder(TargetRadius, TargetHeight);

	// AcceptanceRadius를 자신의 콜라이더 반경과 목표 액터의 콜라이더 반경을 합한 값으로 설정
	float AcceptanceRadius = /*SelfRadius +*/ TargetRadius /*+ 5.0f*/; // 여유 거리 5.0f 추가
	
	
	// AI 컨트롤러가 목표 액터로 이동하도록 명령
	EPathFollowingRequestResult::Type MoveToResult = AIController->MoveToActor(TargetActor, AcceptanceRadius, true, true, false, 0, true);

	if (MoveToResult == EPathFollowingRequestResult::Type::Failed)
	{
		return EBTNodeResult::Failed;
	}

	// 이동 완료 시 호출될 델리게이트 바인딩
	AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTTask_MoveToCustom::OnMoveCompleted);

	// NodeMemory에 OwnerComp를 저장하여 나중에 FinishLatentTask에서 사용할 수 있도록 저장
	FBTTask_MoveToCustomMemory* MyMemory = CastInstanceNodeMemory<FBTTask_MoveToCustomMemory>(NodeMemory);
	MyMemory->CachedOwnerComp = &OwnerComp;

	
	return EBTNodeResult::InProgress;
}

uint16 UBTTask_MoveToCustom::GetInstanceMemorySize() const
{
	return sizeof(FBTTask_MoveToCustomMemory);
}

void UBTTask_MoveToCustom::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// if (CachedOwnerComp)
	// {
	// 	FBTEQSServiceMemory* MyMemory = reinterpret_cast(BTComp->GetNodeMemory(this, BTComp->FindInstanceContainingNode(this)));
	// 	
	// 	// 이동이 성공적으로 완료되었는지 여부에 따라 결과 설정
	// 	const EBTNodeResult::Type TaskResult = Result.IsSuccess() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	// 	FinishLatentTask(*CachedOwnerComp, TaskResult);
	// }
}
