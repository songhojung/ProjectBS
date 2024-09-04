// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SoldierBaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "SpawnArea.generated.h"

UCLASS()
class PROJECTBS_API ASpawnArea : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE ETeamType GetTeamType() { return TeamType; }
	
protected:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn")
	ETeamType TeamType;

};
