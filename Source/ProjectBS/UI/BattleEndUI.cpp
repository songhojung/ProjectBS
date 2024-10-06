// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleEndUI.h"

#include "ProjectBSGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UIManager.h"

void UBattleEndUI::NativeConstruct()
{
	Super::NativeConstruct();


	Button_NextLevel->OnClicked.AddDynamic(this, &UBattleEndUI::ButtonNextLevelClicked);
	Button_GoToMenu->OnClicked.AddDynamic(this, &UBattleEndUI::ButtonGotoMenuClicked);
}

void UBattleEndUI::SetUI(ETeamType teamType)
{
	Empty();
	if(teamType ==	ETeamType::OwnTeam)
	{
		Text_Win->SetVisibility(ESlateVisibility::Visible);
		Button_NextLevel->SetVisibility(ESlateVisibility::Visible);
		Button_GoToMenu->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_Lose->SetVisibility(ESlateVisibility::Visible);
		Button_GoToMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleEndUI::ButtonNextLevelClicked()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		int levelId = gameIns->GetGameLevelId();
		int nextLeveId = levelId + 1;
		
		if(gameIns->CheckGameLevelId(nextLeveId))
		{
			gameIns->SetGameLevelId(nextLeveId);
			gameIns->GameStart(nextLeveId);

			UUIManager::Get()->RemoveUI(this);

		}

	}
	
}

void UBattleEndUI::ButtonGotoMenuClicked()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		int levelId = gameIns->GetGameLevelId();
		gameIns->GameStart(levelId);
	}
}

void UBattleEndUI::Empty()
{
	Text_Win->SetVisibility(ESlateVisibility::Collapsed);
	Text_Lose->SetVisibility(ESlateVisibility::Collapsed);
	Button_NextLevel->SetVisibility(ESlateVisibility::Collapsed);
	Button_GoToMenu->SetVisibility(ESlateVisibility::Collapsed);
}
