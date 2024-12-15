// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterWorldMapNode.h"

#include "ChapterWorldMapUI.h"
#include "Components/Button.h"
#include "Manager/UIManager.h"

void UChapterWorldMapNode::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Selected->OnClicked.AddDynamic(this, &UChapterWorldMapNode::ClickSelectedButton);

	Button_Unselected->OnClicked.AddDynamic(this, &UChapterWorldMapNode::ClickUnselectedButton);

}

void UChapterWorldMapNode::SetMapNode()
{
}

void UChapterWorldMapNode::SelectedNode(int nodeId)
{
	bool isSelected = NodeId == nodeId;
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
		worldmapUI->SetCurrentSelectedNode(NodeId);
	}
}
