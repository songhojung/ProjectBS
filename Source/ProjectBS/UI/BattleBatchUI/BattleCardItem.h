// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleCardItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBattleCardItem : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
protected:
	
	// UPROPERTY(meta = (BindWidget))
	// class UButton* Button_Slot;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_CharCard;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Cost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Hp;

	// (Slot으로 설정해 드래그 시 Slot이 그대로 마우스를 따라다니게 할 것)
	UPROPERTY(EditAnywhere, Category = "DragItem")
	TSubclassOf<UBattleCardItem> DragWidgetClass;

protected:

	uint32 CharId;

public:
	void SetItem(int charId);
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	// virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	UFUNCTION()
	void  ClickedButtonItem();


};
