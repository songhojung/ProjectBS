// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"

#include "Characters/SoldierBaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = ProjectileMesh;
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> meshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	// if(meshRef.Object)
	// {
	// 	ProjectileMesh ->SetStaticMesh(meshRef.Object);
	// }

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// ProjectileMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("StaticMesh")));
	// ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this ,&AProjectileBase::OnOverlap);
	ProjectileMesh->OnComponentHit.AddDynamic(this,&AProjectileBase::OnHit);
	
}

void AProjectileBase::SetCollisionProfileName(ETeamType teamType)
{
	//팀별로 콜리젼 프로필 다르게 설정
	FName teamName = teamType == ETeamType::OwnTeam ? TEXT("SbCollisionOwn") : TEXT("SbCollisionOther");
	ProjectileMesh->SetCollisionProfileName(teamName);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// 발사체가 자신을 발사한 액터가 아닌 다른 Pawn과 충돌했는지 확인
	if (OtherActor && OtherActor != this && OtherActor->IsA(ASoldierBaseCharacter::StaticClass()))
	{
		// 원하는 충돌 처리 코드 작성
		UE_LOG(LogTemp, Warning, TEXT("Projectile hit a Pawn: %s"), *OtherActor->GetName());

		// // 데미지를 가하는 코드
		// 		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		// TakeDamage 직접 호출
		FDamageEvent DamageEvent; // 기본 데미지 이벤트 사용 (추가 세부 정보가 필요하면 커스텀 이벤트 사용 가능)
		OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);

		Destroy();
	}
}


// void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// 발사체가 자신이 아닌 다른 Pawn과 겹쳤을 때 실행
// 	if (OtherActor && OtherActor != this && OtherActor->IsA(ASoldierBaseCharacter::StaticClass()))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Projectile overlapped with ASoldierBaseCharacter: %s"), *OtherActor->GetName());
//
// 		// 데미지를 가하는 코드
// 		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
// 		
// 		// 필요한 처리를 수행 (예: 데미지를 입히거나 발사체 파괴)
// 		Destroy();
// 	}
// }




