// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFieldManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UGameFieldManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UGameFieldManager();
public:
	// Static function to simplify access to the subsystem (Singleton style)
	static UGameFieldManager* Get(UObject* WorldContextObject);
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	UClass* SoldierClass = nullptr;


protected:
	void OnWorldBeginPlay();

public:
	void StartBattleInField(int32 forceCount);
	
};
