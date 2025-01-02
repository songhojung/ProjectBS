// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterListItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UChapterListItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Select;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* Text_ChapterName;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* Image_Lock;

	int ChapterId;

public:
	void SetItem(int chapterId);

	void SelctedChapterItem(int chapterId);
	UFUNCTION()
	void ClickSelectedButton();
};
