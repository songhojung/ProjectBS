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

private:
	int32 CurrentSeletedNodeId = 0;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node)
	TArray<class UChapterWorldMapNode*> WorldMapNodes;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Play;



public:
	void SetCurrentSelectedNode(int32 nodeId);
	
protected:
	UFUNCTION()
	void OnClickButtonPlay();
};
