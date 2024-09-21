// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "Characters/SoldierBaseCharacter.h"
#include "GameFieldManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UGameFieldManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	UGameFieldManager();
public:
	// Static function to simplify access to the subsystem (Singleton style)
	static UGameFieldManager* Get(UObject* WorldContextObject);
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGameFieldManager, STATGROUP_Tickables); }
	
protected:
	UClass* SoldierClass = nullptr;

	bool CanTick = true;

	int32 LastHoverMouseBatchGridIndex = -1;

	TWeakObjectPtr<class APlayerController> PlayerController;
	
	TWeakObjectPtr<class ABatchGridActor> BatchGrid;

	class ASoldierBaseCharacter* BatchGridSoldier;

	TArray<ASoldierBaseCharacter*> SoldierArray;

	TMap<int32,bool> OwnTeamBatchGridAssignedMap;
	
protected:
	void OnWorldBeginPlay();
	void TrackMouseOnPlane();
	ASoldierBaseCharacter* CreateSoldier(int32 soldierId, FVector location, ETeamType teamType);

public:
	void StartBattleInField(int32 forceCount);

	void BatchSoldier(int32 soldierId, FVector location, ETeamType teamType);

	class ABatchGridActor* GetBatchGrid();

	FORCEINLINE bool IsContainGridIndex(int32 gridIndex) {return OwnTeamBatchGridAssignedMap.Contains(gridIndex);} 
};
