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
	static ConstructorHelpers::FObjectFinder<UDataTable> SoldierStatTableRef(TEXT("/Script/Engine.DataTable'/Game/GameData/SoldierStatTable.SoldierStatTable'"));
	if(SoldierStatTableRef.Object)
	{
		const UDataTable* dataTble = SoldierStatTableRef.Object;
		check(dataTble->GetRowMap().Num() > 0);

		TArray<uint8*>valueArray;
		dataTble->GetRowMap().GenerateValueArray(valueArray);

		Algo::Transform(valueArray,SoldierStatTables,
			[](uint8* value)
		{
			return *reinterpret_cast<FSoldierStatData*>(value);
		});

	}
	
}
