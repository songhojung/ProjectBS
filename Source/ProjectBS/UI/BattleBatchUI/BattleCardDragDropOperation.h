// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "BattleCardDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBattleCardDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int CharId;
	
	
};
