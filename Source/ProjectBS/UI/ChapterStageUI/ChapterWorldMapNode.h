// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ChapterWorldMapNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTBS_API UChapterWorldMapNode : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node)
	int NodeId;


	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Selected;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UButton* Button_Unselected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node)
	TArray<UImage*> Fogs;

public:
	void SetMapNode();

	void SelectedNode(int nodeId);
	
	UFUNCTION()
	void ClickSelectedButton();

	UFUNCTION()
	void ClickUnselectedButton();
};
