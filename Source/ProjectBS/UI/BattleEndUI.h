// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/SoldierBaseCharacter.h"
#include "BattleEndUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBattleEndUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetUI(ETeamType teamType);
	
	UFUNCTION()
	void  ButtonNextLevelClicked();

	UFUNCTION()
	void  ButtonGotoMenuClicked();
protected:
	void Empty();
	
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Win;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Lose;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_NextLevel;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_GoToMenu;
};
