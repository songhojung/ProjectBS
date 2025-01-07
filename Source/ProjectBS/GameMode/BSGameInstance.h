// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CharacterProperty/TeamComponent.h"
#include "BSGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FBattleCostChanged, int32, int32);
/**
 * 
 */
UCLASS()
class PROJECTBS_API UBSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBSGameInstance();

	virtual void Init() override;
	void OnPostLoadMap(UWorld* loadedWorld);

protected:

	UPROPERTY(visibleAnywhere)
	TMap<ETeamType,int32> UnitsByTeamMap;
	uint8 bNeedLevelLoadedProcess : 1;

	
public:
	uint8 bGameStarted : 1;
	
	int32 GameLevelId;

	int32 UsedBattleCost;
	
	uint8 bBattleStarted : 1;
	uint8 bBattleEnd : 1;

	ETeamType BattleWinTeam;

	FBattleCostChanged OnBattleCostChanged;

protected:
	void InitializeChapterPlayData();
	
public:
	FORCEINLINE void SetGameStartedFlag(uint8 _bGameStarted) { bGameStarted = _bGameStarted; }
	FORCEINLINE bool IsGameStarted() const { return bGameStarted; }
	FORCEINLINE bool IsBattleStarted() const { return bBattleStarted; }
	
	FORCEINLINE int32 GetGameLevelId() {return GameLevelId;}
	
	FORCEINLINE int32 GetUsedBattleCost() {return UsedBattleCost;}
	FORCEINLINE void AddBattleCost(int32 addValue) {UsedBattleCost += addValue; OnBattleCostChanged.Broadcast(UsedBattleCost , GetMaxBattleCost());}
	FORCEINLINE void RemoveBattleCost(int32 removeValue) {UsedBattleCost = FMath::Clamp(UsedBattleCost - removeValue, 0, UsedBattleCost); OnBattleCostChanged.Broadcast(UsedBattleCost,GetMaxBattleCost());}
	int32 GetMaxBattleCost();
	void ClearBattleCost() {UsedBattleCost = 0;}
	bool IsEnoughBattleCost( int charId);
	
public:
	void SetGameLevelId(int32 newGameLevelId) {GameLevelId = newGameLevelId;} 
	
	bool CheckGameLevelId(int32 gameLevelId);
	
	void GameStart(int32 gameLevelId);

	void PostGameLevelLoaded();
	
	void BattleStart(int32 count);

	void OnDeadUnit(ETeamType teamType, int32 count);

	void BattleEnd();

	void LoadLevel(int32 gameLevelId, bool& outChangedLevel);
};
