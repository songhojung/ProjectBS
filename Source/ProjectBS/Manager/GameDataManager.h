// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/SoldierStatData.h"
#include "GameDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UGameDataManager : public UObject
{
	GENERATED_BODY()

	static UGameDataManager* Instance;

public:
	static UGameDataManager* Get();

private:
	UGameDataManager();

public:
	FORCEINLINE const FSoldierStatData& GetSoldierStatData(int id) {return SoldierStatTables[id - 1]; } 

private:
	TArray<FSoldierStatData> SoldierStatTables;
	
};
