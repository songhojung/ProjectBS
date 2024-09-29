// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/SoldierCharData.h"
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

	template <typename StructType>
	void LoadDataTable(const FString& TableName, TArray<StructType>& OutArray);
public:
	FORCEINLINE const FSoldierStatData* GetSoldierStatData(int id) ;
	FORCEINLINE const FSoldierCharData* GetSoldierCharData(int id);

private:
	TArray<FSoldierStatData> SoldierStatTables;
	TArray<FSoldierCharData> SoldierCharTables;
	
};


