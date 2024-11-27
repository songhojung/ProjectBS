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

public:
	// 블루프린트의 PlaySoundEffect 함수 호출
	UFUNCTION(BlueprintImplementableEvent, Category = "Sound")
	void PlaySoundEffect(bool isWin);


	
public:
	
	UFUNCTION()
	void  ButtonNextLevelClicked();

	UFUNCTION()
	void  ButtonGotoMenuClicked();
protected:
	void Empty();
	
	
protected:
	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UTextBlock* Text_Win;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* Text_Lose;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_NextLevel;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_GoToMenu;
};
