// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameViewPawn.generated.h"

UCLASS()
class PROJECTBS_API AGameViewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameViewPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<class USpringArmComponent> CamSpringArm;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<class UCameraComponent> MainCamemra;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	TObjectPtr<class UInputAction> InputActionCameraZoom;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	TObjectPtr<class UInputMappingContext> MappingContext;

protected:
	UFUNCTION()
	void CameraZoom(const FInputActionValue& Value);


};
