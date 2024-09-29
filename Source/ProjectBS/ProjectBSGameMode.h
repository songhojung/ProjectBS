// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterProperty/TeamComponent.h"
#include "ProjectBSGameMode.generated.h"

UCLASS(minimalapi)
class AProjectBSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectBSGameMode();

protected:

	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Batch Grid")
	TSubclassOf<class ABatchGridActor> BatchGridClass;

	UPROPERTY(visibleAnywhere)
	TMap<ETeamType,int32> UnitsByTeamMap;
	
public:
	int32 GameLevel;

	uint8 bBattleEnd : 1;

	ETeamType BattleWinTeam;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Batch Grid")
	TObjectPtr<class ABatchGridActor> BatchGrid;

public:
	void SetGameLevel(int32 newGameLevel) {GameLevel = newGameLevel;} 

	void GameStart(int32 gameLevel);

	void BattleStart(int32 count);

	void OnDeadUnit(ETeamType teamType, int32 count);

	void BattleEnd();
	
};



