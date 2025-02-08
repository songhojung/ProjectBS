// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectUI/StageScrollItem.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameMode/BSGameInstance.h"
#include "Manager/UIManager.h"
#include "UserData/PlayDataSaveGame.h"
#include "UserData/SaveGameSubsystem.h"


void UStageScrollItem::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Play->OnClicked.AddDynamic(this, &UStageScrollItem::OnClickButtonPlay);
}

void UStageScrollItem::SetItem(int chapterId, const FLevelStageData* stageData)
{

	if(stageData!=nullptr)
	{
		UPlayDataSaveGame* playSaveData = USaveGameSubsystem::LoadSavedGameData<UPlayDataSaveGame>(this,TEXT("PlayData"),0);


		int stageId =stageData->Id;
		StageId = stageId;
		ChapterId = chapterId;
		
		FString thumnailName = stageData->StageThumnail;
		//챕터 번호
		FString chapterNumText = FString::Printf(TEXT("Stage.%d"),stageId);
		Text_StageNum->SetText(FText::FromString(chapterNumText));

		//챕터 썸네일
		FString texturePath = FString::Printf(TEXT("/Game/UIResource/%s.%s"),*thumnailName,*thumnailName );
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, *texturePath);
		
		if (NewTexture && Image_StageTumbnail)
		{
			// Brush를 생성하여 이미지를 설정
			Image_StageTumbnail->SetBrushFromTexture(NewTexture);
		}
		
		//챕터 이름
		Text_StageName->SetText(FText::FromString(stageData->StageTitleName));

		//챕터 설명
		Text_StageDesc->SetText(FText::FromString(stageData->StageDescription));


	
		FChapterStageClearData* chapterStageData = playSaveData->GetChapterStageClearData(chapterId);
		bool isLock = chapterStageData->ClearStageIds.Contains(stageId) == false;
		LockMark->SetVisibility(isLock ?ESlateVisibility::Visible : ESlateVisibility::Collapsed );
	}


	
}

void UStageScrollItem::OnClickButtonPlay()
{
	
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		gameIns->GameStart(ChapterId,StageId);
	}
	
	UUIManager::Get()->RemoveUI(this);
}
