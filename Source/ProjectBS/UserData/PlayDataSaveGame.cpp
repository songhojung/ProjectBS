// Fill out your copyright notice in the Description page of Project Settings.


#include "UserData/PlayDataSaveGame.h"


void UPlayDataSaveGame::SetChapterStageClearData(int chapterId, int clearStageId)
{
	FChapterStageClearData* chpaterData = GetChapterStageClearData(chapterId);

	if(chpaterData == nullptr)
	{
		FChapterStageClearData newChapterData;
		
		newChapterData.ChapterId = chapterId;
		
		if(newChapterData.ClearStageIds.Contains(clearStageId) == false)
			newChapterData.ClearStageIds.Add(clearStageId);

		ChapterStageClearDatas.Add(newChapterData);
	}
	else
	{
		chpaterData->ClearStageIds.Add(clearStageId);
	}
}

FChapterStageClearData* UPlayDataSaveGame::GetChapterStageClearData(int chapterId)
{
	FChapterStageClearData* chapterStageData =  ChapterStageClearDatas.FindByPredicate([chapterId](const FChapterStageClearData& data)
	{
		return	chapterId == data.ChapterId;
	});

	return chapterStageData;
}
