// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserData/SaveGameBase.h"
#include "PlayDataSaveGame.generated.h"


USTRUCT()
struct FChapterStageClearData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	int ChapterId;

	UPROPERTY()
	TArray<int> ClearStageIds = {1};
};

/**
 * 
 */
UCLASS()
class PROJECTBS_API UPlayDataSaveGame : public USaveGameBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int LastChapterId = 1;

	UPROPERTY()
	TArray<FChapterStageClearData> ChapterStageClearDatas;


public:
	void SetChapterStageClearData(int chapterId, int clearStageId);

public:
	FChapterStageClearData* GetChapterStageClearData(int chapterId);
};
