// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "ToastPopupUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UToastPopupUI : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Toast;

	FTimerHandle Timer;

protected:
	void DestroyPopup();
	
public:

	void ShowPopup(FString text); 
};
