// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"
#include "TitleUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UTitleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	
protected:
	int32 ForceCount = 1; //병력 숫자
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Start;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_Num;

	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void TextBoxTextChanged(const FText& Text, ETextCommit::Type CommitMethod);
};
