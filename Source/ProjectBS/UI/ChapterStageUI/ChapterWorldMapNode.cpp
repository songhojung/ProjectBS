// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterWorldMapNode.h"

#include "ChapterWorldMapUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameData/ChapterData.h"
#include "Manager/GameDataManager.h"
#include "Manager/UIManager.h"

void UChapterWorldMapNode::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Selected->OnClicked.AddDynamic(this, &UChapterWorldMapNode::ClickSelectedButton);

	Button_Unselected->OnClicked.AddDynamic(this, &UChapterWorldMapNode::ClickUnselectedButton);

}

void UChapterWorldMapNode::SetMapNode()
{
	const FChapterData* chapterData = UGameDataManager::Get()->GetChapterData(ChapterId);
	if(chapterData!=nullptr)
	{
		//챕터이름
		FString chapterName = FString::Printf(TEXT("Chap.%d %s"),ChapterId,*chapterData->ChapterName);
		Text_ChapterName->SetText(FText::FromString(chapterName));
	}
}

void UChapterWorldMapNode::SelectedNode(int nodeId)
{
	bool isSelected = ChapterId == nodeId;
	Button_Selected->SetVisibility(isSelected ? ESlateVisibility::Visible: ESlateVisibility::Collapsed);
	Button_Unselected->SetVisibility(isSelected== false ? ESlateVisibility::Visible: ESlateVisibility::Collapsed);
}


void UChapterWorldMapNode::ClickSelectedButton()
{

}

void UChapterWorldMapNode::ClickUnselectedButton()
{
	UChapterWorldMapUI* worldmapUI =  Cast<UChapterWorldMapUI>(UUIManager::Get()->GetUI("ChapterWorldMapUI"));
	if(worldmapUI!=nullptr)
	{
		worldmapUI->SetCurrentSelectedNode(ChapterId);
	}
}
