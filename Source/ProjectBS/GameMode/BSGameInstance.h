// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CharacterProperty/TeamComponent.h"
#include "BSGameInstance.generated.h"

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
	
public:
	uint8 bGameStarted : 1;
	
	int32 GameLevelId;

	uint8 bBattleEnd : 1;

	ETeamType BattleWinTeam;
public:
	FORCEINLINE void SetGameStartedFlag(uint8 _bGameStarted) { bGameStarted = _bGameStarted; }
	FORCEINLINE bool IsGameStarted() const { return bGameStarted; }
	FORCEINLINE int32 GetGameLevelId() {return GameLevelId;}
	
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