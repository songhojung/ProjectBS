// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectGameModeUI.h"
#include "Components/Button.h"
#include "GameMode/BSGameInstance.h"
#include "Manager/UIManager.h"


void USelectGameModeUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Back->OnClicked.AddDynamic(this, &USelectGameModeUI::OnClickButtonBack);
	Button_CampaignMode->OnClicked.AddDynamic(this, &USelectGameModeUI::OnClickButtonCampaignMode);
	Button_MultiplayMode->OnClicked.AddDynamic(this, &USelectGameModeUI::OnClickButtonMultiplayMode);
}

void USelectGameModeUI::OnClickButtonBack()
{
	//타이틀 UI 노출
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController!=nullptr)
	{
		UUIManager::Get()->AddUI(TEXT("TitleUI"),playerController);

		UUIManager::Get()->RemoveUI(this);
	}
}

void USelectGameModeUI::OnClickButtonCampaignMode()
{
	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
	{
		gameIns->GameStart(gameIns->GetGameLevelId());
	}

	UUIManager::Get()->RemoveUI(this);


}

void USelectGameModeUI::OnClickButtonMultiplayMode()
{
}
