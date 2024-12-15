// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterWorldMapUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UChapterWorldMapUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Play;


protected:
	UFUNCTION()
	void OnClickButtonPlay();
};
