// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameFieldManager.h"

#include "GameDataManager.h"
#include "CharacterProperty/TeamComponent.h"
#include "Characters/SoldierBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

UGameFieldManager::UGameFieldManager()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> CharacterBlueprint(TEXT("Blueprint'/Game/ProjectBS/Blueprints/BP_SoldierBase.BP_SoldierBase'"));

	if(CharacterBlueprint.Object)
	{
		SoldierClass = CharacterBlueprint.Object->GeneratedClass;
		
	}
}

UGameFieldManager* UGameFieldManager::Get(UObject* WorldContextObject)
{
	if (const UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		return GameInstance->GetSubsystem<UGameFieldManager>();
	}
	return nullptr;
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
	// UE_LOG(LogTemp, Warning, TEXT("@@@@OnWorldBeginPlay"));
	//
	// TArray<AActor*> actors;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), actors);
	//
	// for (AActor* actor : actors)
	// {
	// 	ASpawnArea* SpawnArea = Cast<ASpawnArea>(actor);
	// 	if(SpawnArea!= nullptr)
	// 	{
	// 		for (int i = 0 ; i < 5; i++)
	// 		{
	// 			ASoldierBaseCharacter* soldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( SpawnArea->GetActorRotation(), SpawnArea->GetActorLocation()));
	// 			soldier->SetTeam(SpawnArea->GetTeamType());
	// 			soldier->SetStat(100.f,50.f);
	//
	// 			UGameplayStatics::FinishSpawningActor(soldier,FTransform( SpawnArea->GetActorRotation(), SpawnArea->GetActorLocation()));
	// 		}
	//
	// 		FString teamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
	// 		UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s"), *teamStr );
	//
	// 	}
	// 		
	// }
	
}

void UGameFieldManager::StartBattleInField(int32 forceCount)
{

	UE_LOG(LogTemp, Warning, TEXT("@@@@StartBattleInField"));

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ASpawnArea* SpawnArea = Cast<ASpawnArea>(actor);
		if(SpawnArea!= nullptr)
		{
			FSoldierStatData statData = UGameDataManager::Get()->GetSoldierStatData(1);
			
			for (int i = 0 ; i < forceCount; i++)
			{
				int row = i / 10;
				int column = i % 10;
				FVector SpawnLocation = SpawnArea->GetActorLocation() + FVector(row * 100, column * 100, 0); 
				ASoldierBaseCharacter* soldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( SpawnArea->GetActorRotation(), SpawnLocation));
				soldier->SetTeam(SpawnArea->GetTeamType());
				soldier->SetStat(statData);

				UGameplayStatics::FinishSpawningActor(soldier,FTransform( SpawnArea->GetActorRotation(), SpawnLocation));
			}

			FString teamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
			UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s"), *teamStr );

		}
			
	}
}
