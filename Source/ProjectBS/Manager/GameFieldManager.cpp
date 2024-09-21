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

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
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

	//배치 병사 안보이게
	if(BatchGridSoldier!=nullptr)
		BatchGridSoldier->SetActorHiddenInGame(true);

	//TICK 처리 안하게
	CanTick = false;
}

void UGameFieldManager::BatchSoldier(int32 soldierId, FVector location, ETeamType teamType)
{
	ASoldierBaseCharacter* soldier = CreateSoldier(soldierId, location, teamType);

	//생성된 병사 컨테이너에 ADD
	SoldierArray.Add(soldier);

	int32 row;
	int32 col;
	BatchGrid->LocationToTile(location,row,col);
	//해당 열, 행에 대한 그리드 인덱스 반환
	int32 gridIndex = BatchGrid->GetGridIndex(row,col); 

	// 그리드 인덱스 (그리드 칸 하나 마다 0부터 부여된 인덱스) 포함 안되었다면 포함하도록 ADD
	if(OwnTeamBatchGridAssignedMap.Contains(gridIndex) ==false)
		OwnTeamBatchGridAssignedMap.Add(gridIndex,true);
}

ASoldierBaseCharacter* UGameFieldManager::CreateSoldier(int32 soldierId, FVector location, ETeamType teamType)
{
	//병사 오브젝트 로드해서 위치시킨다
	FSoldierStatData statData = UGameDataManager::Get()->GetSoldierStatData(soldierId);
	FVector spawnLocation = location;
	FRotator spawnRotation = FRotator(0, 0, 0);
	ASoldierBaseCharacter* soldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( spawnRotation, spawnLocation));
	soldier->SetTeam(teamType);
	soldier->SetStat(statData);

	float height = soldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	spawnLocation = FVector(location.X, location.Y, height);
	UGameplayStatics::FinishSpawningActor(soldier,FTransform( spawnRotation, spawnLocation));

	return soldier;
}

void UGameFieldManager::TrackMouseOnPlane()
{
	// APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if(PlayerController == nullptr)
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	FHitResult Hit;
	if(PlayerController.IsValid()&& PlayerController.Get()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{
		ABatchGridActor* grid =  GetBatchGrid();

		if(grid !=nullptr)
		{
			int32 row;
			int32 col;
			grid->LocationToTile(Hit.Location,row,col);
			int32 gridIndex = grid->GetGridIndex(row,col);

			if(LastHoverMouseBatchGridIndex == gridIndex)
				return;

			LastHoverMouseBatchGridIndex = gridIndex;
			
			bool isContainGridIndex = OwnTeamBatchGridAssignedMap.Contains(gridIndex);
			
			grid->SetSelectedTile(row,col,!isContainGridIndex);

			FVector2d gridPos;
			bool isLocate = grid->TileToGridLocation(row,col,true,gridPos);
			if(isLocate)
			{
				UE_LOG(LogTemp, Warning, TEXT("@@@@TrackMouseOnPlane %i / %i"), row, col);

				if(BatchGridSoldier == nullptr)
				{
					//병사 오브젝트 로드해서 위치시킨다
					BatchGridSoldier = CreateSoldier(1,FVector(gridPos.X,gridPos.Y,0.f),ETeamType::OwnTeam);
					BatchGridSoldier->SetActorEnableCollision(false);

					UE_LOG(LogTemp, Warning, TEXT("!@!@!@TrackMouseOnPlane CreateSoldier %i / %i"), row, col);

				}
				else
				{
					float height = BatchGridSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					BatchGridSoldier->SetActorHiddenInGame(false);
					BatchGridSoldier->SetActorEnableCollision(false);
					BatchGridSoldier->SetActorLocation(FVector(gridPos.X, gridPos.Y, height));

					UE_LOG(LogTemp, Warning, TEXT("####TrackMouseOnPlane Move other grid %i / %i"), row, col);

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
