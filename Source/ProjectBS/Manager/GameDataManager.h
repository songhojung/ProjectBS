// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/LevelStageData.h"
#include "GameData/SoldierCharData.h"
#include "GameData/SoldierStatData.h"
#include "GameData/ChapterData.h"
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
	const FSoldierStatData* GetSoldierStatData(int id) ;
	
	const FSoldierCharData* GetSoldierCharData(int id);
	TArray<FSoldierCharData> GetAllOwnSoldierCharData();
	
	const FLevelStageData* GetLevelStageData(int id);
	
	const FChapterData* GetChapterData(int id);
	FORCEINLINE TArray<FChapterData> GetAllChapterDatas(){return ChapterTables;}

	//특정스테이지의 적병력의 병력이름과 숫자반환
	TTuple<TArray<FString>,TArray<int32>> GetSoldierNumsInfoInLevelStage(int levelStageId);

private:
	TArray<FSoldierStatData> SoldierStatTables;
	TArray<FSoldierCharData> SoldierCharTables;
	TArray<FLevelStageData> LevelStageTables;
	TArray<FChapterData> ChapterTables;
	
};


