// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChapterStageUI/ChapterWorldMapUI.h"

#include "Components/Button.h"
#include "GameMode/BSGameInstance.h"
#include "Manager/UIManager.h"


void UChapterWorldMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Play->OnClicked.AddDynamic(this, &UChapterWorldMapUI::OnClickButtonPlay);

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
