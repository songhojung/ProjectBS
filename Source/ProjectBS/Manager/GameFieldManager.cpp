// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameFieldManager.h"
#include "Characters/SoldierBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

UGameFieldManager::UGameFieldManager()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> CharacterBlueprint(TEXT("Blueprint'/Game/ProjectBS/Blueprints/BP_SoldierBase.BP_SoldierBase'"));

	if(CharacterBlueprint.Object)
	{
		SoldierClass = CharacterBlueprint.Object->GeneratedClass;
		
	}
}

void UGameFieldManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// FActorSpawnParameters spawnParams;
	// spawnParams.Owner = this;
	// spawnParams.Instigator = GetInstigator();
	GetWorld()->OnWorldBeginPlay.AddUObject(this,&UGameFieldManager::OnWorldBeginPlay);

	
	
}

void UGameFieldManager::OnWorldBeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("@@@@OnWorldBeginPlay"));

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("OwnTeamSpawnPos"),Actors);
	if(Actors.Num() > 0)
	{
		AActor* OwnTeamSpawnPos = Actors[0];

		UE_LOG(LogTemp, Warning, TEXT("@@@@OnWorldBeginPlay Actors.Num() > 0"));

		// FActorSpawnParameters SpawnParams;
		for (int i = 0 ; i < 5; i++)
		{
			ASoldierBaseCharacter* soldier = GetWorld()->SpawnActor<ASoldierBaseCharacter>(SoldierClass, OwnTeamSpawnPos->GetActorLocation(), OwnTeamSpawnPos->GetActorRotation());

		}
	}
}
