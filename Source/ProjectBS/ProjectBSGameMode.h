// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
// protected:
// 	TSubclassOf<UUserWidget> StartingGameUserWidgetClass;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Widget")
// 	TObjectPtr<UUserWidget> CurrentUserWidget;
// 	
// public:
// 	void ChangeUserWidget(TSubclassOf<UUserWidget> newUserWidet);
public:
	int32 GameLevel;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Batch Grid")
	TObjectPtr<class ABatchGridActor> BatchGrid;

public:
	void SetGameLevel(int32 newGameLevel) {GameLevel = newGameLevel;} 

	void GameStart(int32 gameLevel);

	void BattleStart(int32 count);
	
};



