// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectBSCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "BatchGrid/BatchGridActor.h"
#include "Engine/LocalPlayer.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectBSPlayerController::AProjectBSPlayerController()
{

	static  ConstructorHelpers::FObjectFinder<UInputMappingContext> defaultMappingContextRef(TEXT("/Game/TopDown/Input/IMC_Default.IMC_Default"));
	if (defaultMappingContextRef.Succeeded())
	{
		DefaultMappingContext = defaultMappingContextRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> clickInputActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/TopDown/Input/Actions/IA_SetDestination_Click.IA_SetDestination_Click'"));
	if(clickInputActionRef.Object)
	{
		SetDestinationClickAction = clickInputActionRef.Object;
	}

	//Test
	static ConstructorHelpers::FObjectFinder<UInputAction> testActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectBS/Input/Action/IA_TestInput.IA_TestInput'"));
	if(testActionRef.Object)
	{
		TestKeyAction = testActionRef.Object;
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AProjectBSPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	
	if(gameIns)
	{
		//레벨 로드시 로드 완료 콜백호출
		// gameIns->PostGameLevelLoaded();
		
		// if(gameIns->IsGameStarted() ==false)
		// {
		// 	//게임 시작 플레그 설정
		// 	gameIns->SetGameStartedFlag(true);
		// 	
		// 	//타이틀 UI 노출
		// 	UUIManager::Get()->AddUI(TEXT("TitleUI"),this);
		//
		//
		// }
	}
}

void AProjectBSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectBSPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectBSPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectBSPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectBSPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProjectBSPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProjectBSPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProjectBSPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProjectBSPlayerController::OnTouchReleased);

		
		EnhancedInputComponent->BindAction(TestKeyAction, ETriggerEvent::Triggered, this, &AProjectBSPlayerController::OnTestInputTriggered);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void AProjectBSPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AProjectBSPlayerController::OnSetDestinationTriggered()
{

	

	

}

void AProjectBSPlayerController::OnSetDestinationReleased()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns != nullptr && gameIns->IsBattleStarted())
		return;

	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;

		// ABatchGridActor* grid =  UGameFieldManager::Get(this)->GetBatchGrid();
		// if(grid!=nullptr)
		// {
		// 	int32 row;
		// 	int32 col;
		// 	FVector2d outGridCenterLocation;
		// 	grid->LocationToTile(Hit.Location,row,col);
		// 	grid->TileToGridLocation(row,col,true, outGridCenterLocation);
		//
		// 	int32 gridIndex = grid->GetGridIndex(row,col);

			// if(UGameFieldManager::Get(this)->IsContainGridIndex(gridIndex)==false)
			// 	UGameFieldManager::Get(this)->BatchSoldier(FVector(outGridCenterLocation.X,outGridCenterLocation.Y,0.f), ETeamType::OwnTeam);

			// if(UGameFieldManager::Get(this)->IsContainGridIndex(gridIndex)==false)
				// UGameFieldManager::Get(this)->BatchSoldier(FVector(Hit.Location.X,Hit.Location.Y,0.f), ETeamType::OwnTeam);

		// }
	}
	else
	{

	}
}

// Triggered every frame when the input is held down
void AProjectBSPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AProjectBSPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AProjectBSPlayerController::OnTestInputTriggered()
{
	UGameFieldManager::Get(this)->GetActorForceOwnTeamTestCode();
}



