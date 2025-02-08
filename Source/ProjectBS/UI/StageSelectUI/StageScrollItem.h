// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/LevelStageData.h"
#include "StageScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UStageScrollItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	int ChapterId;
	int StageId;
	
public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* Text_StageNum;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* Image_StageTumbnail;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* Text_StageName;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* Text_StageDesc;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UOverlay* LockMark;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Play;
	
	
public:
	void SetItem(int chapterId, const FLevelStageData* stageData);

protected:
	UFUNCTION()
	void OnClickButtonPlay();
};
