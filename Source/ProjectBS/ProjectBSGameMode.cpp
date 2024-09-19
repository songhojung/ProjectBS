// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBSGameMode.h"
#include "ProjectBSPlayerController.h"
#include "ProjectBSCharacter.h"
#include "BatchGrid/BatchGridActor.h"
#include "GameMode/GameViewPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"
#include "UObject/ConstructorHelpers.h"

AProjectBSGameMode::AProjectBSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectBSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/ProjectBS/Blueprints/BP_GameView.BP_GameView_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Script/Engine.Blueprint'/Game/ProjectBS/Blueprints/BP_BSPlayerController.BP_BSPlayerController'"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AProjectBSGameMode::BeginPlay()
{
	Super::BeginPlay();

	
	GameStart(1);
	
}

void AProjectBSGameMode::GameStart(int32 gameLevel)
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController)
	{
		// UUIManager::Get()->AddUI(TEXT("TitleUI"),playerController);
		//배치 UI 노출
		UUIManager::Get()->AddUI(TEXT("BattleBatchUI"),playerController);

		//필드에 배치 grid 생성
		if(BatchGrid==nullptr)
		{
			FVector2d gridCenterLocation;
			FVector location = FVector(0.f,0.f,5.f);
			FRotator rotation = FRotator(0.f,0.f,0.f);
			
			BatchGrid =  GetWorld()->SpawnActorDeferred<ABatchGridActor>(BatchGridClass,FTransform(rotation,location));
			BatchGrid->SetNumRowsAndColumes(10,10);
			BatchGrid->TileToGridLocation(BatchGrid->GetNumRows() / 2, BatchGrid->GetNumColumns() / 2 , false ,gridCenterLocation);
			BatchGrid->MakeBatchGrid();
			UGameplayStatics::FinishSpawningActor(BatchGrid,FTransform( rotation, FVector(location.X,-gridCenterLocation.Y,location.Z)));

		}
		else
		{
			BatchGrid->SetActiveBatchGrid(true);
		}
	}
}

void AProjectBSGameMode::BattleStart(int32 count)
{
	// 게임필드 에 필드에 병력, 오브젝트 등 로드.
	UGameFieldManager::Get(this)->StartBattleInField(count);

	//배치 그리드 비활성
	if(BatchGrid)
		BatchGrid->SetActiveBatchGrid(false);
}


// void AProjectBSGameMode::ChangeUserWidget(TSubclassOf<UUserWidget> newUserWidet)
// {
// }
