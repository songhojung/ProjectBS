// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectGameModeUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API USelectGameModeUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Back;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_CampaignMode;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_MultiplayMode;


public:
	UFUNCTION()
	void OnClickButtonBack();
	UFUNCTION()
	void OnClickButtonCampaignMode();
	UFUNCTION()
	void OnClickButtonMultiplayMode();


};
