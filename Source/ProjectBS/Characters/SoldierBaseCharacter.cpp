// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierBaseCharacter.h"

#include "AI/SoldierAIController.h"

// Sets default values
ASoldierBaseCharacter::ASoldierBaseCharacter()
{

	AIControllerClass = ASoldierAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}





