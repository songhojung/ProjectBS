// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterWorldMapUI.h"

#include "ChapterWorldMapNode.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "GameMode/BSGameInstance.h"
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

}

void UChapterWorldMapUI::SetCurrentSelectedNode(int32 nodeId)
{
	CurrentSeletedNodeId = nodeId;

	for (auto node : WorldMapNodes)
	{
		node->SelectedNode(CurrentSeletedNodeId);
	}
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
