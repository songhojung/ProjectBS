// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleUI.h"

#include "ToastPopupUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameMode/BSGameInstance.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"

void UTitleUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Start->OnClicked.AddDynamic(this,&UTitleUI::StartButtonClicked);
}

void UTitleUI::StartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));

	
	// UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	// if(gameIns)
	// {
	// 	gameIns->GameStart(gameIns->GetGameLevelId());
	// }

	//타이틀 UI 노출
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController!=nullptr)
	{
		UUIManager::Get()->AddUI(TEXT("SelectGameModeUI"),playerController);
	}
	
	UUIManager::Get()->RemoveUI(this);


}

