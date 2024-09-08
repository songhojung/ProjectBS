// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"

void UTitleUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Start->OnClicked.AddDynamic(this,&UTitleUI::StartButtonClicked);
	TextBox_Num->OnTextCommitted.AddDynamic(this,&UTitleUI::TextBoxTextChanged);

	TextBox_Num->SetText(FText::FromString(FString::FromInt(ForceCount)));
}

void UTitleUI::StartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));

	// 게임필드 에 필드에 배틀 시작 처리 관련 함수 호출
	UGameFieldManager::Get(this)->StartBattleInField(ForceCount);

	UUIManager::Get()->RemoveUI(this);
}

void UTitleUI::TextBoxTextChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString StringValue = Text.ToString();  // FText -> FString로 변환하여 저장
	ForceCount = FCString::Atoi(*StringValue);
}
