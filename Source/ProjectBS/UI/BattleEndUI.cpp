// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleEndUI.h"

#include "ProjectBSGameMode.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UIManager.h"

void UBattleEndUI::NativeConstruct()
{
	Super::NativeConstruct();


	Button_NextLevel->OnClicked.AddDynamic(this, &UBattleEndUI::ButtonNextLevelClicked);
	Button_Retry->OnClicked.AddDynamic(this, &UBattleEndUI::ButtonNextLevelClicked);
	Button_GoToMenu->OnClicked.AddDynamic(this, &UBattleEndUI::ButtonGotoMenuClicked);
}

void UBattleEndUI::SetUI(ETeamType teamType)
{
	Empty();
	if(teamType ==	ETeamType::OwnTeam)
	{
		WinPanel->SetVisibility(ESlateVisibility::Visible);
		Button_NextLevel->SetVisibility(ESlateVisibility::Visible);
		Button_Retry->SetVisibility(ESlateVisibility::Visible);
		Button_GoToMenu->SetVisibility(ESlateVisibility::Visible);
		PlaySoundEffect(true);
	}
	else
	{
		LosePanel->SetVisibility(ESlateVisibility::Visible);
		Button_Retry->SetVisibility(ESlateVisibility::Visible);
		Button_GoToMenu->SetVisibility(ESlateVisibility::Visible);
		PlaySoundEffect(false);

	}
}

void UBattleEndUI::ButtonNextLevelClicked()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		int curChapterId=gameIns->GetGameChapterId(); 
		int curStageId = gameIns->GetGameStageId();
		int nextStageId = curStageId + 1;
		
		if(gameIns->CheckGameStageId(nextStageId))
		{
			gameIns->GameStart(curChapterId,nextStageId);

			UUIManager::Get()->RemoveUI(this);

		}

	}
	
}

void UBattleEndUI::ButtonRetryClicked()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		int curChapterId=gameIns->GetGameChapterId(); 
		int curStageId = gameIns->GetGameStageId();
		
		if(gameIns->CheckGameStageId(curStageId))
		{
			gameIns->GameStart(curChapterId,curStageId);

			UUIManager::Get()->RemoveUI(this);

		}

	}
}

void UBattleEndUI::ButtonGotoMenuClicked()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController)
	{
		UUIManager::Get()->RemoveUI(this);
		
		UUIManager::Get()->AddUI(TEXT("SelectGameModeUI"),playerController);
	}
}

void UBattleEndUI::Empty()
{
	WinPanel->SetVisibility(ESlateVisibility::Collapsed);
	LosePanel->SetVisibility(ESlateVisibility::Collapsed);
	Button_Retry->SetVisibility(ESlateVisibility::Collapsed);
	Button_NextLevel->SetVisibility(ESlateVisibility::Collapsed);
	Button_GoToMenu->SetVisibility(ESlateVisibility::Collapsed);
}
