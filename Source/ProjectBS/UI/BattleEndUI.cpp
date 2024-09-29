// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleEndUI.h"

#include "ProjectBSGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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
	//게임모드에게 유닛 죽음 호출
	AProjectBSGameMode* gameMode = Cast<AProjectBSGameMode>(UGameplayStatics::GetGameMode(this));
	if(gameMode)
	{
		gameMode->GameStart(1);
	}
	
	UUIManager::Get()->RemoveUI(this);
}

void UBattleEndUI::ButtonGotoMenuClicked()
{
	//게임모드에게 유닛 죽음 호출
	AProjectBSGameMode* gameMode = Cast<AProjectBSGameMode>(UGameplayStatics::GetGameMode(this));
	if(gameMode)
	{
		gameMode->GameStart(1);
	}
	
	UUIManager::Get()->RemoveUI(this);
}

void UBattleEndUI::Empty()
{
	Text_Win->SetVisibility(ESlateVisibility::Collapsed);
	Text_Lose->SetVisibility(ESlateVisibility::Collapsed);
	Button_NextLevel->SetVisibility(ESlateVisibility::Collapsed);
	Button_GoToMenu->SetVisibility(ESlateVisibility::Collapsed);
}
