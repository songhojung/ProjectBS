// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameFieldManager.h"

#include "GameDataManager.h"
#include "UIManager.h"
#include "BatchGrid/BatchGridActor.h"
#include "CharacterProperty/TeamComponent.h"
#include "Characters/SoldierBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"
#include "UI/ToastPopupUI.h"

class UToastPopupUI;

UGameFieldManager::UGameFieldManager()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> CharacterBlueprint(TEXT("Blueprint'/Game/ProjectBS/Blueprints/BP_SoldierBase.BP_SoldierBase'"));

	if(CharacterBlueprint.Object)
	{
		SoldierClass = CharacterBlueprint.Object->GeneratedClass;
		
	}

	CanTick = false;
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

	// 에디터에서 실행 중인 경우 초기화를 하지 않음
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Skipping Subsystem Initialization in Editor."));
		return;
	}
	
	if (UWorld* World = GetWorld())
	{
		World->OnWorldBeginPlay.AddUObject(this, &UGameFieldManager::OnWorldBeginPlay);
	}
	
}

void UGameFieldManager::OnWorldBeginPlay()
{
	SetSpawnAreas();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);


}

void UGameFieldManager::SetSpawnAreas()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ASpawnArea* spawnArea = Cast<ASpawnArea>(actor);

		if(spawnArea)
			SpawnAreaArray.Add(spawnArea);
	}
}

void UGameFieldManager::Tick(float DeltaTime)
{
	// TrackSampleSoldierMouseOnBatchGrid();
	// TrackSampleSoldierMouseOnGround();
}

bool UGameFieldManager::IsTickable() const
{
	// 에디터에서는 Tick을 막고, 게임 실행 시에만 Tick을 허용
	if (!GetWorld() || !GetWorld()->IsGameWorld())
	{
		return false;  // 에디터에서는 Tick을 중지
	}
	

	
	return CanTick;
}




void UGameFieldManager::StartBattleInField(int32 levelStageId)
{

	UE_LOG(LogTemp, Warning, TEXT("@@@@StartBattleInField"));

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ASpawnArea* SpawnArea = Cast<ASpawnArea>(actor);
		if(SpawnArea!= nullptr && SpawnArea->GetTeamType() == ETeamType::EnemyTeam)
		{
			const FLevelStageData* stageData = UGameDataManager::Get()->GetLevelStageData(levelStageId);
			check(stageData != nullptr);


			for (int i = 0 ; i < stageData->EnemyTeamCharNum; i++)
			{
				int enemyCharId = stageData->EnemyTeamBatchCharIds[i];

				const FSoldierCharData* charData = UGameDataManager::Get()->GetSoldierCharData(enemyCharId);
				check(charData != nullptr);
				const FSoldierStatData* statData = UGameDataManager::Get()->GetSoldierStatData(charData->StatId);
				check(statData != nullptr);
				
				int row = i / 10;
				int column = i % 10;
				FVector SpawnLocation = SpawnArea->GetActorLocation() + FVector(row * 150, column * 150, 0); 
				// ASoldierBaseCharacter* soldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(SoldierClass, FTransform( SpawnArea->GetActorRotation(), SpawnLocation));
				// soldier->SetTeam(SpawnArea->GetTeamType());
				// soldier->SetStat(*statData);
				//
				// UGameplayStatics::FinishSpawningActor(soldier,FTransform( SpawnArea->GetActorRotation(), SpawnLocation));

				ASoldierBaseCharacter* soldier = CreateSoldier(charData->Id,SpawnLocation,SpawnArea->GetActorRotation(),ETeamType::EnemyTeam);
				
				//AI 시작
				soldier->GetAIController()->StartAI();

				
				OtherSoldierArray.Add(soldier);
			}

			FString teamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
			UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s"), *teamStr );

		}
			
	}


	//자신팀 병력도 AI 시작
	for (auto soldier : OwnSoldierArray)
	{
		soldier->GetAIController()->StartAI();
	}

	//배치 병사 바로 파괴
	if(BatchGridSampleSoldier!=nullptr)
		DestroySampleBatchSoldier();

	//TICK 처리 안하게
	CanTick = false;

}

void UGameFieldManager::BatchSoldier(FVector location, ETeamType teamType)
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetWorld()->GetGameInstance());
	if(gameIns == nullptr)
		return;

	//병력 배치비용 부족하면 리턴
	if(gameIns->IsEnoughBattleCost(TargetBatchSoliderCharId)== false)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!!!BattleCost Full => %i "), gameIns->GetUsedBattleCost());
		
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		UUIManager::Get()->AddPopupUI(TEXT("ToastPopupUI"),playerController,FCompletedAddUIDelegate::CreateLambda([&](UUserWidget* widget)
		{
			UToastPopupUI* toast = Cast<UToastPopupUI>(widget);
				if(toast)
					toast->ShowPopup(TEXT("The Cost has exceeded the limit"));
		}));
		return;
	}
	
	ASpawnArea* teamSpawnArea = GetTeamSpawnArea(teamType);

	FVector spawnLocation = teamSpawnArea->GetActorLocation();
	float distanceWithSpawnArea= FVector::Distance(FVector(location.X,location.Y,0.f),FVector(spawnLocation.X,spawnLocation.Y,0.f));
	//제한 거리보다 너무 멀면 배치 불가
	if(distanceWithSpawnArea > DISTANCE_MIN_BATCH)
	{
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		UUIManager::Get()->AddPopupUI(TEXT("ToastPopupUI"),playerController,FCompletedAddUIDelegate::CreateLambda([&](UUserWidget* widget)
		{
			UToastPopupUI* toast = Cast<UToastPopupUI>(widget);
				if(toast)
					toast->ShowPopup(TEXT("Can't batch at this location, the distance is too far."));
		}));
		return;
	}

	//배치해서  병력이 바라 볼 방향 
	FRotator teamRotation = teamSpawnArea !=nullptr ? teamSpawnArea->GetActorRotation() : FRotator(0.f,0.f,0.f);
	
	ASoldierBaseCharacter* soldier = CreateSoldier(TargetBatchSoliderCharId, location, teamRotation, teamType);

	//생성된 병사 컨테이너에 ADD
	OwnSoldierArray.Add(soldier);

	// int32 row;
	// int32 col;
	// BatchGrid->LocationToTile(location,row,col);
	// //해당 열, 행에 대한 그리드 인덱스 반환
	// int32 gridIndex = BatchGrid->GetGridIndex(row,col); 
	//
	// // 그리드 인덱스 (그리드 칸 하나 마다 0부터 부여된 인덱스) 포함 안되었다면 포함하도록 ADD
	// if(OwnTeamBatchGridAssignedMap.Contains(gridIndex) ==false)
	// 	OwnTeamBatchGridAssignedMap.Add(gridIndex,true);

	//배치비용추가
	const FSoldierCharData* charData = UGameDataManager::Get()->GetSoldierCharData(TargetBatchSoliderCharId);
	if(charData!=nullptr)
		gameIns->AddBattleCost(charData->Cost);
}

void UGameFieldManager::ChangeSampleBatchSoldier(int32 charId)
{
	if(TargetBatchSoliderCharId == charId)
		return;

	//기존꺼 파괴
	if(BatchGridSampleSoldier !=nullptr)
		BatchGridSampleSoldier->Destroy();
	
	//배치해서  병력이 바라 볼 방향 
	ASpawnArea* teamSpawnArea = GetTeamSpawnArea(ETeamType::OwnTeam);
	FRotator teamRotation = teamSpawnArea !=nullptr ? teamSpawnArea->GetActorRotation() : FRotator(0.f,0.f,0.f);
	//병사 오브젝트 로드해서 위치시킨다
	BatchGridSampleSoldier = CreateSoldier(charId,FVector(0.f,0.f,0.f),teamRotation,ETeamType::OwnTeam);
	
	BatchGridSampleSoldier->SetActorHiddenInGame(false);
	BatchGridSampleSoldier->SetActorEnableCollision(false);
}

void UGameFieldManager::DestroySampleBatchSoldier()
{
	if(BatchGridSampleSoldier!=nullptr)
	{
		BatchGridSampleSoldier->Destroy();
		BatchGridSampleSoldier = nullptr;
	}
}

void UGameFieldManager::ClearFieldComponents()
{
	OwnSoldierArray.Empty();
	OtherSoldierArray.Empty();

	OwnTeamBatchGridAssignedMap.Empty();
	SpawnAreaArray.Empty();
}


ASoldierBaseCharacter* UGameFieldManager::CreateSoldier(int32 charId, FVector location, FRotator rotation, ETeamType teamType)
{
	UClass* soldierClass = LoadCharacterClass(charId);

	if(soldierClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("soldierClass is Null => charId :  %d"), charId);
	}
	
	//병사 오브젝트 로드해서 위치시킨다
	const FSoldierCharData* charData = UGameDataManager::Get()->GetSoldierCharData(charId);
	const FSoldierStatData* statData = UGameDataManager::Get()->GetSoldierStatData(charData->StatId);
	FVector spawnLocation = location;
	FRotator spawnRotation = rotation;
	ASoldierBaseCharacter* soldier = GetWorld()->SpawnActorDeferred<ASoldierBaseCharacter>(soldierClass, FTransform( spawnRotation, spawnLocation));
	if(soldier == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("soldier is Null => charId :  %d"), charId);

	}
	soldier->SetTeam(teamType);
	soldier->SetStat(*statData);
	soldier->SetCollisionProfileName(teamType);

	float height = soldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	spawnLocation = FVector(location.X, location.Y, height);
	UGameplayStatics::FinishSpawningActor(soldier,FTransform( spawnRotation, spawnLocation));

	return soldier;
}

//배치용 그리드 액터 생성 (현재 사용안함)
void UGameFieldManager::CreateBatchGridActor()
{
	//필드에 배치 grid 생성
	if(BatchGrid==nullptr)
	{
		FVector2d gridCenterLocation;
		FVector location = FVector(0.f,0.f,5.f);
		FRotator rotation = FRotator(0.f,0.f,0.f);

		UClass* batchGridClass = LoadBatchGridClass();
		BatchGrid =  GetWorld()->SpawnActorDeferred<ABatchGridActor>(batchGridClass,FTransform(rotation,location));
		BatchGrid->SetNumRowsAndColumes(10,10);
		BatchGrid->TileToGridLocation(BatchGrid->GetNumRows() / 2, BatchGrid->GetNumColumns() / 2 , false ,gridCenterLocation);
		BatchGrid->MakeBatchGrid();
		UGameplayStatics::FinishSpawningActor(BatchGrid,FTransform( rotation, FVector(location.X,-gridCenterLocation.Y,location.Z)));

	}
	else
	{
		BatchGrid->SetActiveBatchGrid(true);
	}

	//그리드위 plane 체크 되도록 플레그 true 변경 
	CanTick = true;

}

void UGameFieldManager::ReadyForBatch()
{
	//그리드위 plane 체크 되도록 플레그 true 변경 
	CanTick = true;
}

void UGameFieldManager::TrackSampleSoldierMouseOnBatchGrid()
{
	// APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if(PlayerController == nullptr)
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	FHitResult Hit;
	if(PlayerController.IsValid()&& PlayerController.Get()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{
		ABatchGridActor* grid =  BatchGrid;

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

				if(BatchGridSampleSoldier == nullptr)
				{
					//배치해서  병력이 바라 볼 방향 
					ASpawnArea* teamSpawnArea = GetTeamSpawnArea(ETeamType::OwnTeam);
					FRotator teamRotation = teamSpawnArea !=nullptr ? teamSpawnArea->GetActorRotation() : FRotator(0.f,0.f,0.f);
					//병사 오브젝트 로드해서 위치시킨다
					BatchGridSampleSoldier = CreateSoldier(TargetBatchSoliderCharId,FVector(gridPos.X,gridPos.Y,0.f),teamRotation,ETeamType::OwnTeam);
					BatchGridSampleSoldier->SetActorEnableCollision(false);

					UE_LOG(LogTemp, Warning, TEXT("!@!@!@TrackMouseOnPlane CreateSoldier %i / %i"), row, col);

				}
				else
				{
					float height = BatchGridSampleSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
					BatchGridSampleSoldier->SetActorHiddenInGame(false);
					BatchGridSampleSoldier->SetActorEnableCollision(false);
					BatchGridSampleSoldier->SetActorLocation(FVector(gridPos.X, gridPos.Y, height));

					UE_LOG(LogTemp, Warning, TEXT("####TrackMouseOnPlane Move other grid %i / %i"), row, col);

				}

			}
			else
			{
				if(BatchGridSampleSoldier!=nullptr)
					BatchGridSampleSoldier->SetActorHiddenInGame(true);
			}
			
			// UE_LOG(LogTemp, Warning, TEXT("@@@@TeamSpawn Completed : %s / x: %.2f , y : %.2f, z : %.2f"), *Hit.GetActor()->GetName(), Hit.GetActor()->GetActorLocation().X, Hit.GetActor()->GetActorLocation().Y, Hit.GetActor()->GetActorLocation().Z );
		}
	}
}

void UGameFieldManager::TrackSampleSoldierMouseOnGround()
{

	if(PlayerController == nullptr)
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	FHitResult Hit;
	if(PlayerController.IsValid()&& PlayerController.Get()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{

		ASpawnArea* ownSpawnArea = GetTeamSpawnArea(ETeamType::OwnTeam);

		float distanceWithSpawnArea = 0.f;
		
		if(ownSpawnArea!=nullptr)
		{
			FVector spawnLocation = ownSpawnArea->GetActorLocation();
			distanceWithSpawnArea= FVector::Distance(FVector(Hit.Location.X,Hit.Location.Y,0.f),FVector(spawnLocation.X,spawnLocation.Y,0.f));
			// UE_LOG(LogTemp, Warning, TEXT("####distanceWithSpawnArea : %f"), distanceWithSpawnArea);
		}

		
		if(BatchGridSampleSoldier == nullptr)
		{
			//배치해서  병력이 바라 볼 방향 
			ASpawnArea* teamSpawnArea = GetTeamSpawnArea(ETeamType::OwnTeam);
			FRotator teamRotation = teamSpawnArea !=nullptr ? teamSpawnArea->GetActorRotation() : FRotator(0.f,0.f,0.f);
			//병사 오브젝트 로드해서 위치시킨다
			BatchGridSampleSoldier = CreateSoldier(TargetBatchSoliderCharId,FVector(Hit.Location.X,Hit.Location.Y,0.f),teamRotation,ETeamType::OwnTeam);
			BatchGridSampleSoldier->SetActorEnableCollision(false);
		
		
		}
		else
		{
			if(distanceWithSpawnArea <= DISTANCE_MIN_BATCH)
			{
				float height = BatchGridSampleSoldier->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				BatchGridSampleSoldier->SetActorHiddenInGame(false);
				BatchGridSampleSoldier->SetActorEnableCollision(false);
				BatchGridSampleSoldier->SetActorLocation(FVector(Hit.Location.X, Hit.Location.Y, height));
			}
			else
			{
				//거리가 너무 멀었을때 배치 불가 배치 샘플 색변화

				//우선임시로 배치샘플 안보이게 함
				BatchGridSampleSoldier->SetActorHiddenInGame(true);
			}
		
		}
	}
}

UClass* UGameFieldManager::LoadCharacterClass(int charTableId)
{
	//Blueprint'/Game/ProjectBS/Blueprints/BP_SoldierBase.BP_SoldierBase'
	///Script/Engine.Blueprint'/Game/ProjectBS/Blueprints/Soldiers/BP_Soldier_Jabjong1.BP_Soldier_Jabjong1'
	const FSoldierCharData* charDataPtr = UGameDataManager::Get()->GetSoldierCharData(charTableId);
	// FString classRefPath = FString::Printf(TEXT("Blueprint'/Game/ProjectBS/Blueprints/Soldiers/%s.%s'"),*charDataPtr->ResourceName,*charDataPtr->ResourceName);
	FString classRefPath = FString::Printf(TEXT("Class'/Game/ProjectBS/Blueprints/Soldiers/%s.%s_C'"),*charDataPtr->ResourceName,*charDataPtr->ResourceName);

	UObject* loadedObject = StaticLoadObject(UObject::StaticClass(),nullptr,*classRefPath);
	UClass* loadedBP = Cast<UClass>(loadedObject);
	
	if(loadedBP)
		return loadedBP;
	else
		return nullptr;

	// const FSoldierCharData* charDataPtr = UGameDataManager::Get()->GetSoldierCharData(charTableId);
	// FString classRefPath = FString::Printf(TEXT("BlueprintGeneratedClass'/Game/ProjectBS/Blueprints/Soldiers/%s_C'"), *charDataPtr->ResourceName);
	//
	// UClass* loadedClass = LoadObject<UClass>(nullptr, *classRefPath);
 //    
	// if (!loadedClass)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Failed to load class: %s"), *classRefPath);
	// }
 //    
	// return loadedClass;
}

UClass* UGameFieldManager::LoadBatchGridClass()
{
	///Script/Engine.Blueprint'/Game/ProjectBS/Blueprints/BP_Batch.BP_Batch'
	FString classRefPath = FString::Printf(TEXT("Blueprint'/Game/ProjectBS/Blueprints/BP_Batch.BP_Batch'"));
	UObject* loadedObject = StaticLoadObject(UObject::StaticClass(),nullptr,*classRefPath);
	UBlueprint* loadedBP = Cast<UBlueprint>(loadedObject);

	if(loadedBP)
		return loadedBP->GeneratedClass;
	else
		return nullptr;
	
}

class ASpawnArea* UGameFieldManager::GetTeamSpawnArea(ETeamType teamType)
{
	for (auto spawnArea : SpawnAreaArray)
	{
		if(spawnArea!= nullptr && spawnArea.Get()->GetTeamType() == teamType)
		{
			return spawnArea.Get();
		}
	}
	return nullptr;
}



class ABatchGridActor* UGameFieldManager::GetBatchGrid()
{
	if(BatchGrid != nullptr)
	{
		return  BatchGrid.Get();
	}
	return nullptr;
}


TArray<ASoldierBaseCharacter*> UGameFieldManager::GetTeamSoldierArray(ETeamType teamType)
{
	if(teamType == ETeamType::OwnTeam)
	{
		return OwnSoldierArray;
	}
	else
	{
		return  OtherSoldierArray;	
	}
}

void UGameFieldManager::GetActorForceOwnTeamTestCode()
{
	for (auto Element : OwnSoldierArray)
	{
		Element->ForceFront();
	}
}
