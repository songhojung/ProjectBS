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

	if(findData)
		return findData;
	else
		return nullptr;
}

const FSoldierCharData* UGameDataManager::GetSoldierCharData(int id)
{
	FSoldierCharData* findData = SoldierCharTables.FindByPredicate([id](const FSoldierCharData& data)
	{
		return data.Id == id;
	});

	if(findData)
		return findData;
	else
		return nullptr;
}
