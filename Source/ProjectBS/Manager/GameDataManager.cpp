// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameDataManager.h"

// Static variable initialization
UGameDataManager* UGameDataManager::Instance = nullptr;

UGameDataManager* UGameDataManager::Get()
{
	if(Instance == nullptr)
	{
		Instance = NewObject<UGameDataManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

UGameDataManager::UGameDataManager()
{
	// static ConstructorHelpers::FObjectFinder<UDataTable> SoldierStatTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/SoldierStatTable.SoldierStatTable'"));
 //     	if(SoldierStatTableRef.Object)
 //     	{
 //     		const UDataTable* dataTble = SoldierStatTableRef.Object;
 //     		check(dataTble->GetRowMap().Num() > 0);
 //     
 //     		TArray<uint8*>valueArray;
 //     		dataTble->GetRowMap().GenerateValueArray(valueArray);
 //     
 //     		Algo::Transform(valueArray,SoldierStatTables,
 //     			[](uint8* value)
 //     		{
 //     			return *reinterpret_cast<FSoldierStatData*>(value);
 //     		});
 //     
 //     	}

	LoadDataTable(TEXT("SoldierStatTable"),SoldierStatTables);
	LoadDataTable(TEXT("SoldierCharTable"),SoldierCharTables);
	LoadDataTable(TEXT("LevelStageTable"),LevelStageTables);
	LoadDataTable(TEXT("ChapterTable"),ChapterTables);
	
}



template <typename StructType>
void UGameDataManager::LoadDataTable(const FString& TableName, TArray<StructType>& OutArray)
{
	FString path = FString::Printf(TEXT("/Script/Engine.DataTable'/Game/GameData/%s.%s'"),*TableName,*TableName);
	static ConstructorHelpers::FObjectFinder<UDataTable> tableRef(*path);
	if(tableRef.Object)
	{
		const UDataTable* dataTble = tableRef.Object;
		check(dataTble->GetRowMap().Num() > 0);
     
		TArray<uint8*>valueArray;
		dataTble->GetRowMap().GenerateValueArray(valueArray);
     
		Algo::Transform(valueArray,OutArray,
			[](uint8* value)
		{
			return *reinterpret_cast<StructType*>(value);
		});
     
	}
}

const FSoldierStatData* UGameDataManager::GetSoldierStatData(int id)
{
	FSoldierStatData* findData = SoldierStatTables.FindByPredicate([id](const FSoldierStatData& data)
	{
		return data.Id == id;
	});

	return findData;
}

const FSoldierCharData* UGameDataManager::GetSoldierCharData(int id)
{
	FSoldierCharData* findData = SoldierCharTables.FindByPredicate([id](const FSoldierCharData& data)
	{
		return data.Id == id;
	});

	return findData;
}

const FLevelStageData* UGameDataManager::GetLevelStageData(int id)
{
	FLevelStageData* findData = LevelStageTables.FindByPredicate([id](const FLevelStageData& data)
	{
		return data.Id == id;
	});

	return findData;
}

const FChapterData* UGameDataManager::GetChapterData(int id)
{
	FChapterData* findData = ChapterTables.FindByPredicate([id](const FChapterData& data)
	{
		return data.Id == id;
	});

	return findData;
}

//특정스테이지의 적병력의 병력이름과 숫자반환
TTuple<TArray<FString>,TArray<int32>> UGameDataManager::	GetSoldierNumsInfoInLevelStage(int levelStageId)
{
	TArray<FString> names;
	TArray<int32> nums;
	
	const FLevelStageData* levelData = GetLevelStageData(levelStageId);
	check(levelData != nullptr);

	for (int i = 0; i < levelData->EnemyTeamCharNum; ++i)
	{
		const FSoldierCharData* charData = GetSoldierCharData(levelData->EnemyTeamBatchCharIds[i]);
		check(charData != nullptr);

		if(names.Contains(charData->UnitName)==false)
			names.Add(charData->UnitName);

		int32 index =  names.IndexOfByKey(charData->UnitName);

		if(index > nums.Num())
		{
			nums[index] ++;

		}
		else
		{
			nums.Add(1);
		}
	}

	return MakeTuple(names,nums);
}
