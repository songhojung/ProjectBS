// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterListItem.h"

#include "ChapterWorldMapUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameData/ChapterData.h"
#include "Manager/GameDataManager.h"
#include "Manager/UIManager.h"

void UChapterListItem::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Select->OnClicked.AddDynamic(this,&UChapterListItem::ClickSelectedButton);
}

void UChapterListItem::SetItem(int chapterId)
{
	ChapterId = chapterId;
	
	//Test
	TArray<int> unlockedChapterIds = {1};
	
	const FChapterData* chapterData = UGameDataManager::Get()->GetChapterData(chapterId);
	if(chapterData!=nullptr)
	{
		//챕터이름
		FString chapterName = FString::Printf(TEXT("Chap.%d %s"),chapterId,*chapterData->ChapterName);
		Text_ChapterName->SetText(FText::FromString(chapterName));
	}

	bool isUnlocked = unlockedChapterIds.Contains(chapterId);
	Image_Lock->SetVisibility(isUnlocked ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UChapterListItem::SelctedChapterItem(int chapterId)
{
	Button_Select->SetBackgroundColor( ChapterId== chapterId ? FLinearColor::Green : FLinearColor::Black);
}

void UChapterListItem::ClickSelectedButton()
{
	
	UChapterWorldMapUI* worldmapUI =  Cast<UChapterWorldMapUI>(UUIManager::Get()->GetUI("ChapterWorldMapUI"));
	if(worldmapUI!=nullptr)
	{
		worldmapUI->SetCurrentSelectedNode(ChapterId);
	}
}
