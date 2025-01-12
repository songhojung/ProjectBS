// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectUI/StageSelectUI.h"
#include "UI/StageSelectUI/StageScrollItem.h"
#include "Components/ScrollBox.h"
#include "Manager/GameDataManager.h"

void UStageSelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("UStageSelectUI NativeConstruct !!"));

}

void UStageSelectUI::SetUI(int chapterId)
{
	UE_LOG(LogTemp, Log, TEXT("33 SetUI chapterId : %d "), chapterId);

	ChapterId = chapterId;

	
	const FChapterData* chapterData= UGameDataManager::Get()->GetChapterData(chapterId);
	if(chapterData!=nullptr)
	{
		TArray<int32>stages =  chapterData->StageLevel;
		for (auto stageId : stages)
		{	UE_LOG(LogTemp, Log, TEXT("33 SetUI chapterId : %d "), chapterId);

			const FLevelStageData* stageData = UGameDataManager::Get()->GetLevelStageData(stageId);

			AddScrollItemToScrollBox(stageData);
		}
	}
}

void UStageSelectUI::AddScrollItemToScrollBox(const FLevelStageData* stageData)
{
	UE_LOG(LogTemp, Log, TEXT("11 SetUI AddScrollItemToScrollBox :  "));

	if(StageScrollBox == nullptr )
		return;

	UStageScrollItem* newScrollItemWidget =  CreateWidget<UStageScrollItem>(GetWorld(),StageScrollItemClass);

	if(newScrollItemWidget == nullptr)
		return;
	UE_LOG(LogTemp, Log, TEXT("22 SetUI AddScrollItemToScrollBox :  "));


	newScrollItemWidget->SetDesiredSizeInViewport(FVector2D(357, 513));
	newScrollItemWidget->SetItem(ChapterId,stageData);
	newScrollItemWidget->SetVisibility( ESlateVisibility::Visible);

	StageScrollBox->AddChild( newScrollItemWidget);
}
