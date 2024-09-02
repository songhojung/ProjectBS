// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameViewPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AGameViewPawn::AGameViewPawn()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));;
	
	CamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArmComponent"));
	CamSpringArm->SetupAttachment(RootComponent);

	MainCamemra = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	MainCamemra->SetupAttachment(CamSpringArm);
	
	ConstructorHelpers::FObjectFinder<UInputAction> InputActionCamZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectBS/Input/Action/IA_CameraZoom.IA_CameraZoom'"));
	if(InputActionCamZoomRef.Object)
	{
		InputActionCameraZoom = InputActionCamZoomRef.Object;
	}

	
}

// Called when the game starts or when spawned
void AGameViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameViewPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* playerController = CastChecked<APlayerController>(GetController());

	// Get the enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (Subsystem)
	{
		// Add the input mapping context
		Subsystem->AddMappingContext(MappingContext, 0);
	}
	
	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComp->BindAction(InputActionCameraZoom,ETriggerEvent::Triggered,this, &AGameViewPawn::CameraZoom);

	
}

void AGameViewPawn::CameraZoom(const FInputActionValue& Value)
{
	float zoomValue =  Value.Get<float>();
	
	if(CamSpringArm != nullptr)
	{
		CamSpringArm->TargetArmLength += zoomValue * 100.f;
	}

	if(CamSpringArm->TargetArmLength > 2000.f)
		CamSpringArm->TargetArmLength = 2000.f;
	else if(CamSpringArm->TargetArmLength < 700.f)
		CamSpringArm->TargetArmLength = 700.f;
}



