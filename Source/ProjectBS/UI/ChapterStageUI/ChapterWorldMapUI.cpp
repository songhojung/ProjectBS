// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterWorldMapUI.h"

#include "ChapterListItem.h"
#include "ChapterWorldMapNode.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameData/ChapterData.h"
#include "GameMode/BSGameInstance.h"
#include "Manager/GameDataManager.h"
#include "Manager/UIManager.h"


void UChapterWorldMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentSeletedNodeId = 1;
	
	Button_Play->OnClicked.AddDynamic(this, &UChapterWorldMapUI::OnClickButtonPlay);

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	for (UWidget* Widget : Widgets)
	{
		UChapterWorldMapNode* Node = Cast<UChapterWorldMapNode>(Widget);
		if (Node)
		{
			WorldMapNodes.Add(Node);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d WorldMapNodes"), WorldMapNodes.Num());
	
	for (auto node : WorldMapNodes)
	{
		node->SetMapNode();
		node->SelectedNode(CurrentSeletedNodeId);
	}

	//챕터리스트 아이템 생성
	CreateChapterListItems();

	//첫선택 챕터의 패널 정보설정
	SetChapterInfoPanel(CurrentSeletedNodeId);
}

void UChapterWorldMapUI::SetCurrentSelectedNode(int32 nodeId)
{
	CurrentSeletedNodeId = nodeId;

	for (auto node : WorldMapNodes)
	{
		node->SelectedNode(CurrentSeletedNodeId);
	}

	for (auto chapterItem : ChapterListItems)
	{
		chapterItem->SelctedChapterItem(nodeId);
	}
	
	//챕터 정보 패널 UI설정
	SetChapterInfoPanel(nodeId);
}

void UChapterWorldMapUI::CreateChapterListItems()
{
	TArray<FChapterData> allChapterDatas = UGameDataManager::Get()->GetAllChapterDatas();

	ChapterListItems.Empty();
	for (auto chapterData : allChapterDatas)
	{
		UChapterListItem* item = CreateWidget<UChapterListItem>(GetWorld(),ChapterListItemClass);

		item->SetItem(chapterData.Id);
		item->SelctedChapterItem(CurrentSeletedNodeId);
		
		ChapterListPanel->AddChildToVerticalBox(item);

		ChapterListItems.Add(item);
	}

}

void UChapterWorldMapUI::SetChapterInfoPanel(int chapterId)
{
	const FChapterData* chapterData = UGameDataManager::Get()->GetChapterData(chapterId);
	if(chapterData!=nullptr)
	{
		//챕터 번호
		FString chapterNumText = FString::Printf(TEXT("CHAPTER.%d"),chapterData->Id);
		Text_ChapterNum->SetText(FText::FromString(chapterNumText));

		//챕터 썸네일
		FString texturePath = FString::Printf(TEXT("/Game/UIResource/chapter_thumbnail_%d.chapter_thumbnail_%d"), chapterData->Id, chapterData->Id);
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, *texturePath);

		if (NewTexture && Image_ChapterTumbnail)
		{
			// Brush를 생성하여 이미지를 설정
			Image_ChapterTumbnail->SetBrushFromTexture(NewTexture);
		}
		
		//챕터 이름
		Text_ChapterName->SetText(FText::FromString(chapterData->ChapterName));

		//챕터 설명
		Text_ChapterDesc->SetText(FText::FromString(chapterData->Description));
	}


	//Test
	TArray<int> unlockedChapterIds = {1};

	bool isLock = unlockedChapterIds.Contains(chapterId) == false;
	ChapterInfoLockMark->SetVisibility(isLock ?ESlateVisibility::Visible : ESlateVisibility::Collapsed );
}

void UChapterWorldMapUI::OnClickButtonPlay()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		gameIns->GameStart(gameIns->GetGameLevelId());
	}

	UUIManager::Get()->RemoveUI(this);
}
