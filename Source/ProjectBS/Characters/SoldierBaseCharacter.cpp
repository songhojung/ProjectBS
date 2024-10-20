// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierBaseCharacter.h"

#include "AI/SoldierAIController.h"
#include "CharacterProperty/SoldierStatComponent.h"
#include "CharacterProperty/TeamComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/BSGameInstance.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

// Sets default values
ASoldierBaseCharacter::ASoldierBaseCharacter()
{

	AIControllerClass = ASoldierAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectBS/Animation/AM_Dog_Attack.AM_Dog_Attack'"));
	// if(AttackMontageRef.Object)
	// {
	// 	AttackMontage = AttackMontageRef.Object;
	// }
	//
	// static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("Script/Engine.AnimMontage'/Game/ProjectBS/Animation/AM_Dog_Dead.AM_Dog_Dead'"));
	// if(DeadMontageRef.Object)
	// {
	// 	DeadMontage = DeadMontageRef.Object;
	// }

	TeamComponent = CreateDefaultSubobject<UTeamComponent>(TEXT("SoldierTeamComponent"));

	StatComponent = CreateDefaultSubobject<USoldierStatComponent>(TEXT("SoldierStatsComponent"));

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>("PhysicalAnimationComponent");
}

// Called when the game starts or when spawned
void ASoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsDead = false;

	MeshComponent = GetMesh();

	HitPointStart = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("HitTraceStart")));
	
	HitPointEnd = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("HitTraceEnd")));

	StatComponent->FOnDeadEvent.BindUObject(this, &ASoldierBaseCharacter::SetDead);

	PhysicalAnimation->SetSkeletalMeshComponent(MeshComponent);
	
	// FPhysicalAnimationData 초기화
	// FPhysicalAnimationData PhysicalAnimationData;
	// PhysicalAnimationData.bIsLocalSimulation = true;       // 로컬 시뮬레이션 사용 여부
	// PhysicalAnimationData.OrientationStrength = 1000.0f;    // 회전 제어 강도
	// PhysicalAnimationData.PositionStrength = 1000.0f;       // 위치 제어 강도
	// PhysicalAnimationData.VelocityStrength = 100.0f;       // 속도 제어 강도
	// PhysicalAnimationData.AngularVelocityStrength = 100.0f; // 각속도 제어 강도
	// PhysicalAnimationData.MaxLinearForce = 0.0f;        // 최대 선형 힘
	// PhysicalAnimationData.MaxAngularForce = 0.0f;       // 최대 각력

	////기본캐릭
	// PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow( TEXT("pelvis") ,PhysicalAnimationData);
	//
	// MeshComponent->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"),true,false);

	////강아지캐릭 셋팅
	// PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow( TEXT("spine_001") ,PhysicalAnimationData);
	//
	// MeshComponent->SetAllBodiesBelowSimulatePhysics(TEXT("spine_001"),true,false);


}




void ASoldierBaseCharacter::SetTeam(ETeamType Team)
{
	if(TeamComponent == nullptr)
		return;;

	TeamComponent->SetTeamType(Team);
	TeamComponent->SetTeamDynamicMaterialInstanceOverride(GetMesh(),Team);
	
}

void ASoldierBaseCharacter::SetStat(const FSoldierStatData& statData)
{
	if(StatComponent == nullptr)
		return;

	StatComponent->SetStat(statData);
}

ETeamType ASoldierBaseCharacter::GetTeam() const
{
	return TeamComponent->GetTeamType();
}

AActor& ASoldierBaseCharacter::GetOtherTeamBaseActor() const
{
	return  TeamComponent->GetOtherTeamBaseActor();
}

float ASoldierBaseCharacter::GetAIDectectRange()
{
	return StatComponent->GetDetectRange();
}

bool ASoldierBaseCharacter::IsDead()
{
	return bIsDead;
}


//공격하기
void ASoldierBaseCharacter::Attack()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance != nullptr)
	{
		animInstance->Montage_Play(AttackMontage);
	}
}


//공격시 히트 체크
void ASoldierBaseCharacter::AttackHitCheck()
{
	FHitResult outHitResult;
	FCollisionQueryParams params(SCENE_QUERY_STAT(Attack), false, this);
	float attackRange = 20.f;
	float attackRadius = 30.f;
	// FVector start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	// FVector end  =start + GetActorForwardVector() * attackRange;
	FVector start = HitPointStart->GetComponentLocation();
	FVector end = HitPointEnd->GetComponentLocation();
	FVector SweepDirection = (end - start).GetSafeNormal();
	float CapsuleHalfHeight = FVector::Distance(start,end);

	
	bool hitDetected = GetWorld()->SweepSingleByChannel(outHitResult,start,end,FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeCapsule(attackRadius,CapsuleHalfHeight),params);
	if(hitDetected)
	{
		UE_LOG(LogTemp, Warning, TEXT("hitDetected : %s"), *outHitResult.GetActor()->GetName() );

		FDamageEvent damageEvent;
		// outHitResult.GetActor()->TakeDamage(StatComponent->GetAttackDamange(), damageEvent,GetController(),this);
	}

#if ENABLE_DRAW_DEBUG
	// FVector capsuleOrigin = start + (end - start) * 0.5f;
	FVector capsuleOrigin = ( start + end )/ 2;
	float capsuleHalfHeight = attackRange * 0.5f;
	FColor drawColor = hitDetected ? FColor::Green : FColor::Red;

	FRotator RotatorFromX = FRotationMatrix::MakeFromZ(SweepDirection).Rotator();
	FQuat RotationQuat = RotatorFromX.Quaternion();
	
	DrawDebugCapsule(GetWorld(), capsuleOrigin, CapsuleHalfHeight, attackRange, RotationQuat, drawColor,false, 3.0f);
	// DrawDebugSphere(GetWorld(), End, SphereRadius, 24, bHit ? FColor::Red : FColor::Green, false, 1.0f);

	DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 3.0f, 0, 2.0f);
#endif
}


//데미지 맞을때
float ASoldierBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComponent->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ASoldierBaseCharacter::SetDead()
{
	UE_LOG(LogTemp, Warning, TEXT("$$$$$ 111 Dead Sodlier : %s"), *GetName() );

	bIsDead = true;

	//움직임 없음처리
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	//죽는 에니메이션
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if(animInstance != nullptr)
	{
		//모든 몽타주 중지
		animInstance->StopAllMontages(0.0f);
		animInstance->Montage_Play(DeadMontage,1.0f);
	}

	// AIController에서 AI 중지
	ASoldierAIController* AIController = Cast<ASoldierAIController>(GetController());
	if(AIController)
		AIController->StopAI();

	//액터 충돌체 비활성
	SetActorEnableCollision(false);

	//게임인스턴스에게 유닛 죽음 호출
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		gameIns->OnDeadUnit(GetTeam(),1);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("$$$$$ 222 Dead Sodlier : %s"), *GetName() );

}

void ASoldierBaseCharacter::SetStop()
{
	// AIController에서 AI 중지
	ASoldierAIController* AIController = Cast<ASoldierAIController>(GetController());
	if(AIController)
		AIController->StopAI();
}

void ASoldierBaseCharacter::ForceFront()
{
	 UCharacterMovementComponent* charMovement = GetCharacterMovement();

	
	charMovement->AddForce(GetActorForwardVector() * 5000000.f);
	// LaunchCharacter(GetActorForwardVector() * 1000.f, true, false);
	// charMovement->AddImpulse(GetActorForwardVector() * 10000.f);


	//강아지캐릭일때
		FVector impuse = GetActorForwardVector() * 500000.f;
		GetMesh()->AddImpulseAtLocation(impuse,GetActorLocation() ,TEXT("shoulder_R")); // hand_r , hand_R_end
	
}





