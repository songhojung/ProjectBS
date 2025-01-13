// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/LevelStageData.h"
#include "StageSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UStageSelectUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UScrollBox* StageScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "StageUI")
	TSubclassOf<class UStageScrollItem> StageScrollItemClass;

protected:
	int ChapterId;
public:
	void SetUI(int chapterId);
protected:
	void AddScrollItemToScrollBox(const FLevelStageData* stageData);
	
};
