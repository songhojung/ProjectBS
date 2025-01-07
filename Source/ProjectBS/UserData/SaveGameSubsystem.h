// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
public:
	template<typename T>
	static T* LoadSavedGameData(UObject* obj, const FString& sloatName, int32 userIndex)
	{
		UGameInstance* gameInstance = Cast<UGameInstance>(obj);

		if(gameInstance == nullptr)
		{
			gameInstance = UGameplayStatics::GetGameInstance(obj);
		}
		
		if (gameInstance != nullptr)
		{
			if(USaveGameSubsystem* subsystem = gameInstance->GetSubsystem<USaveGameSubsystem>())
			{
				return  Cast<T>(subsystem->LoadDataLogic(T::StaticClass(), sloatName, userIndex));
			}
		}
		return nullptr;
	}
	
	static  void SaveSaveGameData(const FString& slotName, USaveGameBase* saveGameData);

protected:
	USaveGameBase* LoadDataLogic(UClass* inClass, const FString& slotName, int32 userIndex);
};
