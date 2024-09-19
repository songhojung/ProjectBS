// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleBatchUI.h"

#include "ProjectBSGameMode.h"
#include "TitleUI.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"
#include "UI/BatchUnitItemSlot.h"

void UBattleBatchUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Start->OnClicked.AddDynamic(this, &UBattleBatchUI::StartButtonClicked);

	TextBox_Num->OnTextCommitted.AddDynamic(this,&UBattleBatchUI::TextBoxTextChanged);

	TextBox_Num->SetText(FText::FromString(FString::FromInt(ForceCount)));


	UUniformGridPanel* uniformGridPanel = Cast<UUniformGridPanel>(SoldierUnitGridPanel);
	int colum = 0;
	int row = 0;
	for (int i = 0; i < 12; ++i)
	{
		colum = i % 4;
		row = i / 4;
		UBatchUnitItemSlot* newSlot = CreateWidget<UBatchUnitItemSlot>(GetWorld(),SoldierUnitGridSlotWidgetClass);
		uniformGridPanel->AddChildToUniformGrid(newSlot,row,colum);
	}

}

void UBattleBatchUI::StartButtonClicked()
{



	AProjectBSGameMode* gameMode = Cast<AProjectBSGameMode>(UGameplayStatics::GetGameMode(this));
	if(gameMode)
		gameMode->BattleStart(ForceCount);
	

	UUIManager::Get()->RemoveUI(this);
}

void UBattleBatchUI::TextBoxTextChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString StringValue = Text.ToString();  // FText -> FString로 변환하여 저장
	ForceCount = FCString::Atoi(*StringValue);
}
