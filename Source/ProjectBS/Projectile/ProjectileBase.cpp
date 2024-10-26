// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("StaticMesh")));
	
}



