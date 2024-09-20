// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameFieldManager.h"

#include "GameDataManager.h"
#include "BatchGrid/BatchGridActor.h"
#include "CharacterProperty/TeamComponent.h"
#include "Characters/SoldierBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

UGameFieldManager::UGameFieldManager()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> CharacterBlueprint(TEXT("Blueprint'/Game/ProjectBS/Blueprints/BP_SoldierBase.BP_SoldierBase'"));

	if(CharacterBlueprint.Object)
	{
		SoldierClass = CharacterBlueprint.Object->GeneratedClass;
		
	}

	CanTick = true;
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
	// GetWorld()->OnWorldBeginPlay.AddUObject(this,&UGameFieldManager::OnWorldBeginPlay);

	
	
}

void UGameFieldManager::Tick(float DeltaTime)
{
	TrackMouseOnPlane();
}

bool UGameFieldManager::IsTickable() const
{
	return CanTick;
}




void UGameFieldManager::StartBattleInField(int32 forceCount)
{

	UE_LOG(LogTemp, Warning, TEXT("@@@@StartBattleInField"));

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ASpawnArea* SpawnArea = Cast<ASpawnArea>(actor);
		if(SpawnArea!= nullptr && SpawnArea->GetTeamType() == ETeamType::EnemyTeam)
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

				//AI 시작
				soldier->GetAIController()->StartAI();
			}

			FString teamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
			UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s"), *teamStr );

		}
			
	}


	//자신팀 병력도 AI 시작
	for (auto soldier : SoldierArray)
	{
		soldier->GetAIController()->StartAI();
	}
}

void UGameFieldManager::BatchSoldier(int32 soldierId, FVector location, ETeamType teamType)
{
	ASoldierBaseCharacter* soldier = CreateSoldier(soldierId, location, teamType);

	SoldierArray.Add(soldier);
}

ASoldierBaseCharacter* UGameFieldManager::CreateSoldier(int32 soldierId, FVector location, ETeamType teamType)
{
	//병사 오브젝트 로드해서 위치시킨다
	FSoldierStatData statData = UGameDataManager::Get()->GetSoldierStatData(soldierId);
	FVector SpawnLocation = location;
	FRotator SpawnRotation = FRotator(0, 0, 0);
	BatchGridSoldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( SpawnRotation, SpawnLocation));
	BatchGridSoldier->SetTeam(teamType);
	BatchGridSoldier->SetStat(statData);

	float height = BatchGridSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SpawnLocation = FVector(location.X, location.Y, height);
	UGameplayStatics::FinishSpawningActor(BatchGridSoldier,FTransform( SpawnRotation, SpawnLocation));

	return BatchGridSoldier;
}

void UGameFieldManager::TrackMouseOnPlane()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FHitResult Hit;
	if(playerController && playerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{
		ABatchGridActor* grid =  GetBatchGrid();

		if(grid !=nullptr)
		{
			int32 row;
			int32 col;
			grid->LocationToTile(Hit.Location,row,col);
		
			grid->SetSelectedTile(row,col);

			FVector2d gridPos;
			bool isLocate = grid->TileToGridLocation(row,col,true,gridPos);
			if(isLocate)
			{
				UE_LOG(LogTemp, Warning, TEXT("@@@@TrackMouseOnPlane %i / %i"), row, col);

				if(BatchGridSoldier == nullptr)
				{
					//병사 오브젝트 로드해서 위치시킨다
					BatchGridSoldier = CreateSoldier(1,FVector(gridPos.X,gridPos.Y,0.f),ETeamType::OwnTeam);
					// FSoldierStatData statData = UGameDataManager::Get()->GetSoldierStatData(1);
					// FVector SpawnLocation = FVector(gridPos.X, gridPos.Y, 0);
					// FRotator SpawnRotation = FRotator(0, 0, 0);
					// BatchGridSoldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( SpawnRotation, SpawnLocation));
					// BatchGridSoldier->SetTeam(ETeamType::OwnTeam);
					// BatchGridSoldier->SetStat(statData);
					//
					// float height = BatchGridSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					// SpawnLocation = FVector(gridPos.X, gridPos.Y, height);
					// UGameplayStatics::FinishSpawningActor(BatchGridSoldier,FTransform( SpawnRotation, SpawnLocation));
				}
				else
				{
					float height = BatchGridSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					BatchGridSoldier->SetActorHiddenInGame(false);
					BatchGridSoldier->SetActorLocation(FVector(gridPos.X, gridPos.Y, height));
				}

			}
			else
			{
				if(BatchGridSoldier!=nullptr)
					BatchGridSoldier->SetActorHiddenInGame(true);
			}
			
			// UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s / x: %.2f , y : %.2f, z : %.2f"), *Hit.GetActor()->GetName(), Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z );
		}
	}
}

class ABatchGridActor* UGameFieldManager::GetBatchGrid()
{
	if(BatchGrid.IsValid())
	{
		return  BatchGrid.Get();
	}
	else
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABatchGridActor::StaticClass(), actors);

		if(actors.Num()>0)
		{
			BatchGrid = Cast<ABatchGridActor>(actors[0]);
			return BatchGrid.Get();
		}
	}
	return nullptr;
}
