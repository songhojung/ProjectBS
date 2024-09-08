// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBSGameMode.h"
#include "ProjectBSPlayerController.h"
#include "ProjectBSCharacter.h"
#include "GameMode/GameViewPawn.h"
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

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController)
		UUIManager::Get()->AddUI(TEXT("TitleUI"),playerController);

	
}


// void AProjectBSGameMode::ChangeUserWidget(TSubclassOf<UUserWidget> newUserWidet)
// {
// }
