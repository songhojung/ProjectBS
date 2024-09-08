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
// protected:
// 	TSubclassOf<UUserWidget> StartingGameUserWidgetClass;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Widget")
// 	TObjectPtr<UUserWidget> CurrentUserWidget;
// 	
// public:
// 	void ChangeUserWidget(TSubclassOf<UUserWidget> newUserWidet);
};



