// Fill out your copyright notice in the Description page of Project Settings.


#include "UserData/SaveGameSubsystem.h"

#include "Kismet/GameplayStatics.h"

USaveGameBase* USaveGameSubsystem::LoadDataLogic(UClass* inClass, const FString& slotName, int32 userIndex)
{
	// 찾아보고 없다면 생성
	USaveGameBase* saveData = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(slotName, userIndex));
	if (saveData == nullptr)
	{
		saveData = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(inClass));
	}

	return saveData;
}

void USaveGameSubsystem::SaveSaveGameData(const FString& slotName, USaveGameBase* saveGameData)
{
	saveGameData->SaveSlotName = slotName;
	saveGameData->UserIndex = 0;
	if(UGameplayStatics::SaveGameToSlot(saveGameData, saveGameData->SaveSlotName, saveGameData->UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("!@ Sucessed Saved Game In %s"),*slotName);

	}
}
